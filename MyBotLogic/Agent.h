#pragma once
#include <vector>
#include "Globals.h"
#include "Connector.h"
#include "TurnInfo.h"
#include "State.h"
#include "Node.h"

using namespace std;

class MyBotLogic;
class Agent {
private:
	MyBotLogic * logic;
	int id;
	int goal;
	int pos;
	int nbTurnPassed = 0;
	vector<Connector *> path;
	State * currState;

public:
	void setLogic(MyBotLogic * log) {
		logic = log;
	}
	MyBotLogic * getLogic() {
		return logic;
	}
	int getWaited() {
		return nbTurnPassed;
	}
	void waitTurn() {
		nbTurnPassed++;
	}
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
	void setPath(vector<Connector *>& nPath) {
		path = nPath;
	}
	vector<Connector *>& getPath() {
		return path;
	}
	int getPosAtTurn(int turn) {
		if (turn - nbTurnPassed - 1 < path.size()) {
			return path[turn - nbTurnPassed - 1]->beginNode->getId();
		}
		else {
			return path[path.size() - 1]->endNode->getId();
		}
	}
	Agent(int agentId);
	Action * Play(TurnInfo& _turnInfo);
	void stateChange(TurnInfo& _turnInfo);
};