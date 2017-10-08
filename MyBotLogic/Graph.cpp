#include "Graph.h"
// ? 
#include "ComputeNode.h"
// END ?
#include <algorithm>

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


//TO DO
vector<Connector *> Graph::getPath(int begin, int end)
{
	Node * goal = nodes[end];
	float size = nodes.size()*1.0;
	auto comp = [size](ComputeNode * a, ComputeNode * b) {return a->getEstim() + (a->getNode()->getId() / size) < b->getEstim() + (b->getNode()->getId() / size); };
	vector<ComputeNode *> cNodes;
	set < ComputeNode *, decltype(comp) > searching = set<ComputeNode *, decltype(comp)>(comp);
	set <ComputeNode *> done;
	for each (Node * node in nodes)
	{
		cNodes.push_back(new ComputeNode(node, goal));
	}
	ComputeNode * start = cNodes[begin];
	start->setCout(0);
	searching.insert(cNodes[begin]);
	bool found = false;
	while (!found && searching.size() > 0) {
		ComputeNode * curr = *(searching.begin());
		if (curr->getEntry() != nullptr) {
			cNodes[curr->getEntry()->beginNode->getId()]->setExit(curr->getEntry());
		}
		if (curr->getNode()->getId() == end) {
			found = true;
		}
		else {
			searching.erase(curr);
			done.insert(curr);
			for each (Connector  * con in curr->getNode()->getConnectors())
			{
				ComputeNode * toAdd = cNodes[con->endNode->getId()];
				auto addrInDone = find(done.begin(), done.end(), toAdd);
				auto addrInSearch = find(searching.begin(), searching.end(), toAdd);
				if ((addrInDone == done.end() && addrInSearch == searching.end()) || toAdd->getCout() > curr->getCout() + 1) {
					toAdd->setCout(curr->getCout() + 1);
					toAdd->setEntry(con);
					if (addrInSearch == searching.end()) {
						searching.insert(toAdd);
					}
					if (addrInDone != done.end()) {
						done.erase(toAdd);
					}
				}
			}
		}
	}
	if (!found) {
		//pas de chemin valide, a ecrire quand le cas arrivera
		for each (ComputeNode * cn in cNodes)
		{
			delete cn;
		}
		return vector<Connector *>{};
	}
	else {
		vector<Connector *> result;
		ComputeNode * last = cNodes[begin];
		while (last->getNode()->getId() != end) {
			result.push_back(last->getExit());
			last = cNodes[last->getExit()->endNode->getId()];
		}
		for each (ComputeNode * cn in cNodes)
		{
			delete cn;
		}
		return result;
	}
}

vector<Connector *> Graph::getPath(int begin, int end)
{
	Node * goal = nodes[end];
	float size = nodes.size()*1.0;
	auto comp = [size](ComputeNode * a, ComputeNode * b) {return a->getEstim()+(a->getNode()->getId()/size) < b->getEstim()+ (b->getNode()->getId() / size); };
	vector<ComputeNode *> cNodes;
	set < ComputeNode *, decltype(comp) > searching = set<ComputeNode *, decltype(comp)>(comp);
	set <ComputeNode *> done;
	for each (Node * node in nodes)
	{
		cNodes.push_back(new ComputeNode(node,goal));
	}
	ComputeNode * start = cNodes[begin];
	start->setCout(0);
	searching.insert(cNodes[begin]);
	bool found = false;
	while (!found && searching.size() > 0) {
		ComputeNode * curr = *(searching.begin());
		if (curr->getEntry() != nullptr) {
			cNodes[curr->getEntry()->beginNode->getId()]->setExit(curr->getEntry());
		}
		if (curr->getNode()->getId() == end) {
			found = true;
		}
		else {
			searching.erase(curr);
			done.insert(curr);
			for each (Connector  * con in curr->getNode()->getConnectors())
			{
				ComputeNode * toAdd = cNodes[con->endNode->getId()];
				auto addrInDone = find(done.begin(), done.end(), toAdd);
				auto addrInSearch = find(searching.begin(), searching.end(), toAdd);
				if ((addrInDone == done.end() && addrInSearch == searching.end()) || toAdd->getCout() > curr->getCout() + 1) {
					toAdd->setCout(curr->getCout() + 1);
					toAdd->setEntry(con);
					if (addrInSearch == searching.end()) {
						searching.insert(toAdd);
					}
					if (addrInDone != done.end()) {
						done.erase(toAdd);
					}
				}
			}
		}
	}
	if (!found) {
		//pas de chemin valide, a ecrire quand le cas arrivera
		for each (ComputeNode * cn in cNodes)
		{
			delete cn;
		}
		return vector<Connector *>{};
	}
	else {
		vector<Connector *> result;
		ComputeNode * last = cNodes[begin];
		while (last->getNode()->getId() != end) {
			result.push_back(last->getExit());
			last = cNodes[last->getExit()->endNode->getId()];
		}
		for each (ComputeNode * cn in cNodes)
		{
			delete cn;
		}
		return result;
	}
}

void Graph::init(LevelInfo & levelInfo)
{
	for (int i = 0; i < levelInfo.rowCount; i++) {
		for (int j = 0; j < levelInfo.colCount; j++) {
			nodes.push_back(new Node(i*levelInfo.colCount + j, j, i, levelInfo.tiles[i*levelInfo.colCount + j].tileType));
		}
	}
	for each (Node * node in nodes)
	{
		if (levelInfo.tiles[node->getId()].tileType != Tile::TileAttribute_Forbidden) {
			if (node->getY() % 2 == 0) {
				if (node->getY() > 0) {
					if (node->getX() > 0) {
						node->addConnector(Tile::NW, nodes[(node->getY() - 1)*levelInfo.colCount + node->getX() - 1]);
					}
					node->addConnector(Tile::NE, nodes[(node->getY() - 1)*levelInfo.colCount + node->getX()]);
				}
				if (node->getY() < levelInfo.rowCount - 1) {
					if (node->getX() > 0) {
						node->addConnector(Tile::SW, nodes[(node->getY() + 1)*levelInfo.colCount + node->getX() - 1]);
					}
					node->addConnector(Tile::SE, nodes[(node->getY() + 1)*levelInfo.colCount + node->getX()]);
				}
			}
			else {
				if (node->getY() > 0) {
					if (node->getX() < levelInfo.colCount - 1) {
						node->addConnector(Tile::NE, nodes[(node->getY() - 1)*levelInfo.colCount + node->getX() + 1]);
					}
					node->addConnector(Tile::NW, nodes[(node->getY() - 1)*levelInfo.colCount + node->getX()]);
				}
				if (node->getY() < levelInfo.rowCount - 1) {
					if (node->getX() < levelInfo.colCount - 1) {
						node->addConnector(Tile::SE, nodes[(node->getY() + 1)*levelInfo.colCount + node->getX() + 1]);
					}
					node->addConnector(Tile::SW, nodes[(node->getY() + 1)*levelInfo.colCount + node->getX()]);
				}
			}
			if (node->getX() > 0) {
				node->addConnector(Tile::W, nodes[node->getId() - 1]);
			}
			if (node->getX() < levelInfo.colCount - 1) {
				node->addConnector(Tile::E, nodes[node->getId() + 1]);
			}
		}
	}
}

vector<int> Graph::getGoalPos()
{
	vector<int> result;
	for each (Node * node in nodes)
	{
		if (node->getType() == Tile::TileAttribute_Goal) {
			result.push_back(node->getId());
		}
	}
	return result;
}

Graph::~Graph()
{
	for each (Node * node in nodes)
	{
		delete node;
	}
}
