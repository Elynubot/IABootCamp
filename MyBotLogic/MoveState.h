#pragma once
#include "State.h"
#include "TurnInfo.h"

using namespace std;

class MoveState : public State {

public:
	StateType getTransition(TurnInfo& _turnInfo);
	void onEnter();
	Action * onUpdate(TurnInfo& _turnInfo);
	void onExit();
};