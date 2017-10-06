#pragma once

#include "Globals.h"
#include "TurnInfo.h"

class Agent;
class State {

protected:
	Agent * agent;
public:
	enum StateType {
		NO_CHANGE,
		wait,
		move
	};

	Agent * getAgent() {
		return agent;
	}
	void setAgent(Agent * nAgent) {
		agent = nAgent;
	}
	virtual StateType getTransition(TurnInfo& _turnInfo)=0;
	virtual void onEnter()=0;
	virtual Action * onUpdate(TurnInfo& _turnInfo)=0;
	virtual void onExit()=0;
};