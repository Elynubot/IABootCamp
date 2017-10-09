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
	const Node& GetNode(int id) const noexcept {
		return nodes[id];
	}
	int GetPositionId(int x, int y) const noexcept; 

	//Try to add to the node the connector based on the dir and the x/y of the obj node
	void TryAddConnector(Node& node, Tile::ETilePosition dir, int x, int y) noexcept;

	//Create the nodes without their connectors with the Node constructor
	void CreateNodes(const map<unsigned int, TileInfo>& tiles) noexcept;
	//Create the connectors for each accessible neighbours for each node
	void CreateConnectors(const map<unsigned int, TileInfo>& tiles);

public:
	void Init(int _rowCount, int _colCount, const std::map<unsigned int, TileInfo>& tiles);

	void Update(const map<unsigned int, TileInfo>& tiles) noexcept;
	
private:
	class HeuristicManhattan;
	struct NodeItem;
	class NodeItemPtrComparison;
	class NoPathFound {};
public:
	vector<int> Graph::GetGoalPosition() const noexcept;
	vector<const Connector*> GetPath(int beginId, int goalId) const noexcept;
};


