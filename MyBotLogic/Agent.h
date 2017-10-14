#pragma once
#include <vector>
#include "Globals.h"
#include "Connector.h"
#include "TurnInfo.h"
#include "State.h"
#include "Node.h"

using namespace std;

class Agent {
private:
	int id;
	int goal;
	int pos;
	int nbTurnPassed = 0;
	vector<const Connector *> path;
	State * currState;

	bool pathValid;

public:
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

	void setPathValid(bool nPathValid) {
		pathValid = nPathValid;
	}
	bool getPathValid() {
		return pathValid;
	}

	void setPath(vector<const Connector *>& nPath) {
		path = nPath;
	}
	vector<const Connector *>& getPath() {
		return path;
	}
	int getPosAtTurn(int turn) {
		if (turn - nbTurnPassed - 1 < path.size()) {
			return path[turn - nbTurnPassed - 1]->getBeginNodeC()->getId();
		}
		else {
			return path[path.size() - 1]->getEndNodeC()->getId();
		}
	}
	Agent(int agentId);
	Action * Play(TurnInfo& _turnInfo);
	void stateChange(TurnInfo& _turnInfo);
};