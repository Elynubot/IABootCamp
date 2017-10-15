#include "Graph.h"
#include <algorithm>
#include "Utility.h"

int Graph::getPositionId(int x, int y) const noexcept {
	if ((x >= 0) && (x < colCount*2) && (y >= 0) && (y < rowCount))
		return x / 2 + colCount * y;
	return -1;
}

void Graph::tryAddConnector(Node& node, Tile::ETilePosition dir, int x, int y) noexcept {
	int i = getPositionId(x, y);
	if (i != -1) {
		node.addConnector(dir, &nodes[i]);
	}
}

//Create the nodes without their connectors with the Node constructor
void Graph::createNodes() noexcept {
	for (int i = 0; i < colCount * rowCount; ++i) {
		nodes.push_back(Node{ (unsigned int) i, colCount });
	}
}

//Create the connectors for each accessible neighbours for each node
void Graph::createConnectors() noexcept {
	for_each(nodes.begin(), nodes.end(), [&](Node& node) {
		int x{ node.getX() };
		int y{ node.getY() };
		tryAddConnector(node, Tile::NW, x - 1, y - 1);
		tryAddConnector(node, Tile::NE, x + 1, y - 1);
		tryAddConnector(node, Tile::W, x - 2, y);
		tryAddConnector(node, Tile::E, x + 2, y);
		tryAddConnector(node, Tile::SW, x - 1, y + 1);
		tryAddConnector(node, Tile::SE, x + 1, y + 1);
	});
}

void Graph::init(int _rowCount, int _colCount, const std::map<unsigned int, TileInfo>& tiles, const std::map<unsigned int, ObjectInfo>& objects) noexcept {
	rowCount = _rowCount;
	colCount = _colCount;
	createNodes();
	createConnectors();
	update(tiles, objects);
	popInvalidConnectors();
}

void Graph::updateNodesType(const std::map<unsigned int, TileInfo>& tiles) noexcept {
	for_each(tiles.begin(), tiles.end(), [&](const std::pair<unsigned int, TileInfo>& tile) {
		nodes[tile.second.tileID].setType(tile.second.tileType);
	});
}

void Graph::updateConnectorsWithType(const std::map<unsigned int, TileInfo>& tiles) noexcept {
	for_each(tiles.begin(), tiles.end(), [&](const std::pair<unsigned int, TileInfo>& tile) {
		Node& node = nodes[tile.second.tileID];
		if (node.getType() == Tile::TileAttribute_Forbidden) {
			vector<Connector>* connectors{ node.getConnectors() };
			for_each(connectors->begin(), connectors->end(), [&](Connector& connector) {
				Connector* c1 = connector.getEndNode()->getConnector(connector.getBeginNode());
				if (c1 != nullptr) {
					c1->setIsToDestroy(true);
					invalidConnectors.push_back(Connector{ *c1 });
				}
				connector.setIsToDestroy(true);
				invalidConnectors.push_back(Connector{ connector });
			});
		}
		else if ((node.getType() != Tile::TileAttribute_Forbidden) && (tile.second.tileType == Tile::TileAttribute_Forbidden)) {
			node.setType(tiles.find(node.getId())->second.tileType);
			//Pop old connectors for the neighbours of the node
			vector<Connector>* connectors{ node.getConnectors() };
			for_each(connectors->begin(), connectors->end(), [](Connector& connector) {
				connector.getEndNode()->popConnector(connector.getBeginNode());
			});
			//Clear connectors for the node
			node.clearConnectors();
		}
		node.setType(tile.second.tileType);
	});
}
void Graph::updateConnectorsWithObjects(const std::map<unsigned int, ObjectInfo>& objects) noexcept {
	for_each(objects.begin(), objects.end(), [&](const std::pair<unsigned int, ObjectInfo>& object) {
		const std::set<Object::EObjectType>& objectTypes = object.second.objectTypes;
		if ((objectTypes.find(Object::ObjectType_Wall) != objectTypes.end()) || (objectTypes.find(Object::ObjectType_Window) != objectTypes.end())) {
			Connector* c1 = nodes[object.second.tileID].getConnector(object.second.position);
			if ((c1 != nullptr) && (c1->getIsToDestroy() == false)) {
				c1->setIsToDestroy(true);
				invalidConnectors.push_back(Connector{ *c1 });
				Connector* c2 = c1->getEndNode()->getConnector(c1->getBeginNode());
				if ((c2 != nullptr) && (c2->getIsToDestroy() == false)) {
					c2->setIsToDestroy(true);
					invalidConnectors.push_back(Connector{ *c2 });
				}
			}
		}
	});
}

void Graph::update(const map<unsigned int, TileInfo>& tiles, const std::map<unsigned int, ObjectInfo>& objects) noexcept {
	updateNodesType(tiles);
	updateConnectorsWithType(tiles);
	updateConnectorsWithObjects(objects);
}

class Graph::HeuristicManhattan {
private:
	const Node* goal;
public:
	HeuristicManhattan(const Node* _goal) noexcept
		: goal{ _goal } {}
	int operator()(const Node* node) const noexcept {
		return (abs(goal->getX() - node->getX()) + abs(goal->getY() - node->getY())) / 2;
	}
};

struct Graph::NodeItem {
	Node* ptr;
	NodeItem* previous;
	const Connector* connector;
	int costSoFar;
	int estimatedTotalCost;
};

class Graph::NodeItemPtrComparison {
private:
	bool reverse;
public:
	NodeItemPtrComparison(const bool& revparam = false) : reverse{ revparam } {}
	bool operator() (NodeItem* _lnode, NodeItem* _rnode) const
	{
		if (reverse)
			return (_lnode->estimatedTotalCost < _rnode->estimatedTotalCost);
		else
			return (_lnode->estimatedTotalCost > _rnode->estimatedTotalCost);
	}
};

vector<const Connector*> Graph::getPath(int startId, int goalId) {
	Node* start{ &getNode(startId) };
	Node* end{ &getNode(goalId) };
	//Initialize heuristic
	HeuristicManhattan heuristic{ &getNode(goalId)};

	//Initialize the record for the start node
	NodeItem *startRecord = new NodeItem{};
	startRecord->ptr = start;
	startRecord->previous = nullptr;
	startRecord->connector = nullptr;
	startRecord->costSoFar = 0;
	startRecord->estimatedTotalCost = heuristic(start);

	//Initialize the open and the closed list
	vector<NodeItem*> closedList;
	MyPriorityQueue<NodeItem*, std::vector<NodeItem*>, NodeItemPtrComparison> openList;

	openList.push(startRecord);
	NodeItem* current;
	//Iterate through processing each node
	while (openList.size() > 0) {
		//Find the smallest element in the open list
		current = openList.top();
		openList.pop();
		//If it is the goal node, then terminate
		if (*current->ptr == *end) {
			break;
		}

		//Otherwise get its outgoing connections
		vector<Connector>* neighbours = current->ptr->getConnectors();

		//Loop through each neighbours
		for (auto& neighbour : *neighbours) {

			//Get the cost estimate for the neighbourNode
			const Node* neighbourNode = neighbour.getEndNodeC();
			int neighbourNodeCost = current->costSoFar + 1;

			NodeItem* neighbourRecord = new NodeItem();
			int neighbourNodeHeuristic;

			//Here we find the record in the open list corresponding to the neighbourNode if it exist
			std::vector<NodeItem*>::iterator neighbourRecordOpen = std::find_if(openList.begin(), openList.end(), [&neighbour](NodeItem* ni) ->bool {
				return (*ni->ptr == *neighbour.getEndNodeC());
			});

			//Here we find the record in the closed list corresponding to the neighbourNode if it exist
			std::vector<NodeItem*>::iterator neighbourRecordClose = std::find_if(closedList.begin(), closedList.end(), [&neighbour](NodeItem* ni) -> bool {
				return (*ni->ptr == *neighbour.getEndNodeC());
			});

			//If the node is closed we may have to skip, or remove it from the closed list
			if 
				(neighbourRecordClose != closedList.end()) {

				neighbourRecord = *neighbourRecordClose;

				//if we didn't find a shorter route, skip
				if (neighbourRecord->costSoFar <= neighbourNodeCost)
					continue; //jump to the end of the loop

							  //otherwise remove it from the closed list
				closedList.erase(neighbourRecordClose);

				//we can use the node's old cost values to calculate its heuristic 
				//without calling the possibly expensive heuristic function
				neighbourNodeHeuristic = neighbourRecord->estimatedTotalCost - neighbourRecord->costSoFar;
			}

			//Skip if the node is open and we've not found a better route
			else if 
				(neighbourRecordOpen != openList.end()) {
				
				neighbourRecord = *neighbourRecordOpen;
				//If our route is no better, then skip
				if (neighbourRecord->costSoFar <= neighbourNodeCost)
					continue; //jump to the end of the looop

							  //We can use the node's old cost values to calculate its heuristic 
							  //without calling the possibly expensive heuristic function
				neighbourNodeHeuristic = neighbourRecord->estimatedTotalCost - neighbourRecord->costSoFar;
			}

			//Otherwise we know we've got an unvisited node, so make a record for it
			else {
				neighbourRecord->ptr = neighbour.getEndNode();

				//We'll need to calculate the heuristic value using the function, since we don't have an existing record to use
				neighbourNodeHeuristic = heuristic(neighbourRecord->ptr);
			}

			//We're here if we need to update the node
			//Update the cost, estimate and connection
			neighbourRecord->previous = current;
			neighbourRecord->connector = &neighbour;
			neighbourRecord->costSoFar = neighbourNodeCost;
			neighbourRecord->estimatedTotalCost = neighbourNodeCost + neighbourNodeHeuristic;

			//And add it to the open list
			if (neighbourRecordOpen == openList.end()) {
				openList.push(neighbourRecord);
			}
		}

		//We've finished looking at the connections for the current node, so add it to the closed list and remove it from the openlist
		closedList.push_back(current);
	}

	//Compile the list of connections in the path
	//HERE TALK ABOUT POINTER
	vector<const Connector*> path;

	//We're here if we've either found the goal, or if we've no more nodes to search, find which.
	if (*current->ptr != *end)
		throw NoPathFound{}; //We've run out of nodes without finding the goal, so there's no solution
	else {
		//Work back along the path, accumulating connections
		while (*current->ptr != *start) {
			path.push_back(current->connector);
			current = current->previous;
		}
	}

	//We liberate all the memory allocate for the NodeRecord* in the closedList and in the openList
	std::for_each(closedList.begin(), closedList.end(), [](NodeItem* ni) {
		ni->previous = nullptr;
		delete(ni);
	});

	std::for_each(openList.begin(), openList.end(), [](NodeItem* ni) {
		ni->previous = nullptr;
		delete(ni);
	});

	return path;
}

vector<int> Graph::getGoalPosition() const noexcept
{
	vector<int> result;
	for_each(nodes.begin(), nodes.end(), [&result](const Node& node) {
		if (node.isGoal())
			result.push_back(node.getId());
	});
	return result;
}

void Graph::popInvalidConnectors() noexcept {
	for_each(invalidConnectors.begin(), invalidConnectors.end(), [&](Connector connector) {
		connector.getBeginNode()->popConnector(connector.getEndNode());
	});
	invalidConnectors.clear();
}

vector<const Connector*> Graph::getBestUnkown(int startId) {
	Node* start{ &getNode(startId) };

	NodeItem *startRecord = new NodeItem{};
	startRecord->ptr = start;
	startRecord->previous = nullptr;
	startRecord->connector = nullptr;
	startRecord->costSoFar = 0;

	vector<NodeItem*> closedList;
	MyPriorityQueue<NodeItem*, std::vector<NodeItem*>, NodeItemPtrComparison> openList;

	openList.push(startRecord);
	NodeItem* current;
	//Iterate through processing each node
	while (openList.size() > 0) {
		current = openList.top();
		openList.pop();
		
		//If it is an unknown node, then terminate
		if (current->ptr->getType() == Tile::ETileType::TileAttribute_Unknown) {
			break;
		}

		vector<Connector>* connections = current->ptr->getConnectors();
		NodeItem* connectionRecord;
		//Loop through each neighbours
		for (auto& connection : *connections) {
			const Node* endNode = connection.getEndNodeC();
			int endNodeCost = current->costSoFar + 1;

			std::vector<NodeItem*>::iterator connectionRecordClose = std::find_if(closedList.begin(), closedList.end(), [&connection](NodeItem* ni) -> bool {
				return (*ni->ptr == *connection.getEndNodeC());
			});
			std::vector<NodeItem*>::iterator connectionRecordOpen = std::find_if(openList.begin(), openList.end(), [&connection](NodeItem* ni) -> bool {
				return (*ni->ptr == *connection.getEndNodeC());
			});
			//If the node is closed we have to skip
			if (connectionRecordClose != closedList.end()) {
				continue;
			}
			else if(connectionRecordOpen != openList.end()) {
				connectionRecord = *connectionRecordOpen;
				//if we didn't find a shorter route, skip
				if (connectionRecord->costSoFar <= endNodeCost)
					continue;
			}
			else {
				connectionRecord->ptr = connection.getEndNode();
			}

			connectionRecord->previous = current;
			connectionRecord->connector = &connection;
			connectionRecord->costSoFar = endNodeCost;
			connectionRecord->estimatedTotalCost = endNodeCost;
			//Add it to the open list
			if (connectionRecordOpen == openList.end()) {
				openList.push(connectionRecord);
			}
		}

		closedList.push_back(current);
	}

	vector<const Connector*> path;
	if (current->ptr->getType() != Tile::ETileType::TileAttribute_Unknown)
		throw NoPathFound{};
	else {
		while (*current->ptr != *start) {
			path.push_back(current->connector);
			current = current->previous;
		}
	}

	//We liberate all the memory
	std::for_each(closedList.begin(), closedList.end(), [](NodeItem* ni) {
		ni->previous = nullptr;
		delete(ni);
	});
	std::for_each(openList.begin(), openList.end(), [](NodeItem* ni) {
		ni->previous = nullptr;
		delete(ni);
	});
	return path;
}
