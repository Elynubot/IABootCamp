#include "Graph.h"
#include <algorithm>

//CHANGE POSITION TO NEW FILE
#include <queue>
//According to @skypjack https://stackoverflow.com/questions/41904374/priority-queues-in-c
template<class T, class C = std::vector<T>, class P = std::less<typename C::value_type> >
struct MyPriorityQueue : std::priority_queue<T, C, P> {
	typename C::iterator begin() { return std::priority_queue<T, C, P>::c.begin(); }
	typename C::iterator end() { return std::priority_queue<T, C, P>::c.end(); }
};
//END CHANGE POSITION TO NEW FILE

int Graph::GetPositionId(int x, int y) const noexcept {
	if ((x >= 0) && (x < colCount) && (y >= 0) && (y < rowCount))
		return x / 2 + colCount * y;
	return -1;
}

void Graph::TryAddConnector(Node& node, Tile::ETilePosition dir, int x, int y) noexcept {
	if (int i = GetPositionId(x, y); i != -1) {
		node.AddConnector(dir, &nodes[i]);
	}
}

//Create the nodes without their connectors with the Node constructor
void Graph::CreateNodes(const map<unsigned int, TileInfo>& tiles) noexcept {
	nodes.resize(tiles.size());
	for_each(tiles.begin(), tiles.end(), [&](const pair<unsigned int, TileInfo>& tile) {
		nodes[tile.second.tileID] = Node(tile.second, colCount);
	});
}

//Create the connectors for each accessible neighbours for each node
void Graph::CreateConnectors(const map<unsigned int, TileInfo>& tiles) noexcept {
	for_each(nodes.begin(), nodes.end(), [&](Node& node) {
		int x{ node.GetX() };
		int y{ node.GetY() };
		TryAddConnector(node, Tile::NW, x - 1, y - 1);
		TryAddConnector(node, Tile::NE, x + 1, y - 1);
		TryAddConnector(node, Tile::W, x - 2, y);
		TryAddConnector(node, Tile::E, x + 2, y);
		TryAddConnector(node, Tile::SW, x - 1, y + 1);
		TryAddConnector(node, Tile::SE, x + 1, y + 1);
	});
}

void Graph::Update(const map<unsigned int, TileInfo>& tiles) noexcept {
	for_each(tiles.begin(), tiles.end(), [&](const pair<unsigned int, TileInfo>& tile) {
		Node& node = nodes[tile.second.tileID];
		if ((node.GetType() == Tile::TileAttribute_Forbidden) && (tile.second.tileType != Tile::TileAttribute_Forbidden)) {
			int x{ node.GetX() };
			int y{ node.GetY() };
			//Create new connectors for the node
			TryAddConnector(node, Tile::NW, x - 1, y - 1);
			TryAddConnector(node, Tile::NE, x + 1, y - 1);
			TryAddConnector(node, Tile::W, x - 2, y);
			TryAddConnector(node, Tile::E, x + 2, y);
			TryAddConnector(node, Tile::SW, x - 1, y + 1);
			TryAddConnector(node, Tile::SE, x + 1, y + 1);
			//Create new connectors for the neighbours of the node
			const vector<Connector>* connectors{ node.GetConnectors() };
			for_each(connectors->begin(), connectors->end(), [](Connector& connector) {
				connector.GetEndNode()->AddConnector(connector.GetInvertDirection(), connector.GetBeginNode());
			});
		}
		else if ((node.GetType() != Tile::TileAttribute_Forbidden) && (tile.second.tileType == Tile::TileAttribute_Forbidden)) {
			//Pop old connectors for the neighbours of the node
			const vector<Connector>* connectors{ node.GetConnectors() };
			for_each(connectors->begin(), connectors->end(), [](Connector& connector) {
				connector.GetEndNode()->PopConnector(connector.GetBeginNode());
			});
			//Clear connectors for the node
			node.ClearConnectors();
		}
		node.SetType(tile.second.tileType);
	});
}


void Graph::Init(int _rowCount, int _colCount, const std::map<unsigned int, TileInfo>& tiles) {
	rowCount = _rowCount;
	colCount = _colCount;
	CreateNodes(tiles);
	CreateConnectors(tiles);
}

class Graph::HeuristicManhattan {
private:
	const Node* goal;
public:
	HeuristicManhattan(const Node* _goal) noexcept
		: goal{ _goal } {}
	int operator()(const Node* node) const noexcept {
		return (abs(goal->GetX() - node->GetX()) + abs(goal->GetY() - node->GetY())) / 2;
	}
};

struct Graph::NodeItem {
	const Node* ptr;
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

vector<const Connector*> Graph::GetPath(int startId, int goalId) const {
	const Node* start{ &GetNode(startId) };
	const Node* end{ &GetNode(goalId) };
	//Initialize heuristic
	HeuristicManhattan heuristic{ &GetNode(goalId)};

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
		const vector<Connector>* neighbours = current->ptr->GetConnectors();

		//Loop through each neighbours
		for (auto& neighbour : *neighbours) {

			//Get the cost estimate for the neighbourNode
			const Node* neighbourNode = neighbour.cGetEndNode();
			int neighbourNodeCost = current->costSoFar + 1;

			NodeItem* neighbourRecord = new NodeItem();
			int neighbourNodeHeuristic;

			//Here we find the record in the open list corresponding to the neighbourNode if it exist
			std::vector<NodeItem*>::iterator neighbourRecordOpen = std::find_if(openList.begin(), openList.end(), [&neighbour](NodeItem* ni) ->bool {
				return (*ni->ptr == *neighbour.cGetEndNode());
			});

			//If the node is closed we may have to skip, or remove it from the closed list
			if 
				(//Here we find the record in the closed list corresponding to the neighbourNode if it exist
				std::vector<NodeItem*>::iterator neighbourRecordClose = std::find_if(closedList.begin(), closedList.end(), [&neighbour](NodeItem* ni) -> bool {
					return (*ni->ptr == *neighbour.cGetEndNode());
				}); neighbourRecordClose != closedList.end()) {

				neighbourRecord = *neighbourRecordClose;

				//if we didn't find a shorter route, skip
				if (neighbourRecord->costSoFar <= neighbourNodeCost)
					continue; //jump to the end of the looop

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
				neighbourRecord->ptr = neighbour.cGetEndNode();

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

vector<int> Graph::GetGoalPosition() const noexcept
{
	vector<int> result;
	for_each(nodes.begin(), nodes.end(), [&result](const Node& node) {
		if (node.IsGoal())
			result.push_back(node.GetId());
	});
	return result;
}

