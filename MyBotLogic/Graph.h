#pragma once

#include "Node.h"
#include <map>

// ?
#include "Agent.h"
// END ? 

using namespace std;

class Graph
{
private:	
	int rowCount;
	int colCount;
	vector<Node *> nodes;
public:
	Graph() noexcept;
	~Graph();

private :
	int GetPositionId(int x, int y) const noexcept {
		return y * colCount + x;
	}
	//Create the nodes without their connectors with the Node constructor
	void CreateNodes(const map<unsigned int, TileInfo>& tiles);
	//Create the connectors for each accessible neighbours for each node
	void CreateConnectors(const map<unsigned int, TileInfo>& tiles);
public:
	void Init(int _rowCount, int _colCount, const std::map<unsigned int, TileInfo>& tiles);
	void UpdateNodes(const map<unsigned int, TileInfo>& tiles) noexcept;
	
	
	//TO DO
	float dist(int begin, int end) {
		return dist(nodes[begin], nodes[end]);
	}
	float dist(Node * begin, Node * end) {
		return (abs(begin->getCX()*1.0f - end->getCX()*1.0f) + abs(begin->getCY()*1.0f - end->getCY()*1.0f) + abs(begin->getCZ()*1.0f - end->getCZ()*1.0f)) / 2.0f;
	}
	vector<Connector *> getPath(int begin, int end);
	
	vector<int> getGoalPos();
	
};

