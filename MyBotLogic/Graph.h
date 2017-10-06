#pragma once

#include <vector>
#include "LevelInfo.h"
#include "Node.h"
#include "Agent.h"

using namespace std;

class Graph
{
private:
	vector<Node *> nodes;
public:
	float dist(int begin, int end) {
		return dist(nodes[begin], nodes[end]);
	}
	float dist(Node * begin, Node * end) {
		return (abs(begin->getCX()*1.0f - end->getCX()*1.0f) + abs(begin->getCY()*1.0f - end->getCY()*1.0f) + abs(begin->getCZ()*1.0f - end->getCZ()*1.0f)) / 2.0f;
	}
	vector<Connector *> getPath(int begin, int end);
	void init(LevelInfo & levelInfo);
	vector<int> getGoalPos();
	~Graph();
};

