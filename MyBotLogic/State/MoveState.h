#pragma once
#include "../State.h"
#include "TurnInfo.h"

using namespace std;

class MoveState : public State {

public:
	StateType getTransition(TurnInfo& _turnInfo, Agent * agent);
	void onEnter(Agent * agent);
	Action * onUpdate(TurnInfo& _turnInfo, Agent * agent);
	void onExit(Agent * agent);
};