#pragma once

#include "Node.h"
#include <map>

using namespace std;

class Graph
{
private:
	int rowCount;
	int colCount;
	vector<Node> nodes;

private:
	Node& getNode(int id) noexcept {
		return nodes[id];
	}
	int getPositionId(int x, int y) const noexcept; 

	//Try to add to the node the connector based on the dir and the x/y of the obj node
	void tryAddConnector(Node& node, Tile::ETilePosition dir, int x, int y) noexcept;

	//Create the nodes without their connectors with the Node constructor
	void createNodes(const map<unsigned int, TileInfo>& tiles) noexcept;
	//Create the connectors for each accessible neighbours for each node
	void createConnectors(const map<unsigned int, TileInfo>& tiles) noexcept;

public:
	void init(int _rowCount, int _colCount, const std::map<unsigned int, TileInfo>& tiles);

	void update(const map<unsigned int, TileInfo>& tiles) noexcept;
	
private:
	class HeuristicManhattan;
	struct NodeItem;
	class NodeItemPtrComparison;
	class NoPathFound {};
public:
	vector<int> getGoalPosition() const noexcept;
	vector<const Connector*> getPath(int beginId, int goalId);
	int dist(int n1, int n2) const noexcept {
		const Node* node1{ &nodes[n1] };
		const Node* node2{ &nodes[n2] };
		return (abs(node1->getX() - node2->getX()) + abs(node1->getY() - node2->getY()));
	}
};


