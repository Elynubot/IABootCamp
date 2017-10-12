#pragma once
#include <vector>
#include "Globals.h"
#include "Connector.h"
#include "TurnInfo.h"
#include "State.h"
#include "Node.h"
#include "BehaviourTree/GoalTree.h"

using namespace std;

class Agent {
private:
	int id;
	int goal;
	int pos;
	GoalTree decision;
	vector<const Connector *> path;
	vector<int> forbidden;
	State * currState;

public:
	int getId() {
		return id;
	}
	int getGoal() {
		return goal;
	}
	void setGoal(int nGoal) {
		goal = nGoal;
	}
	int getPos() {
		return pos;
	}
	void setPos(int nPos) {
		pos = nPos;
	}
	void setPath(vector<const Connector *>& nPath) {
		path = nPath;
	}
	vector<const Connector *>& getPath() {
		return path;
	}
	int getNextPos() {
		if (path.size() > 0) {
			return path.back()->getEndNode()->getId();
		}
		else {
			return pos;
		}
	}
	Agent(int agentId);
	void makeDecisions();
	Action * play(TurnInfo& _turnInfo);
	void stateChange(TurnInfo& _turnInfo);
};