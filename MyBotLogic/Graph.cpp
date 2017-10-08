#include "Graph.h"
#include "Globals.h"
#include "ComputeNode.h"

#include <algorithm>

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
}
