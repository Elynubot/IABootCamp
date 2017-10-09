#pragma once
#include "../State.h"
#include "TurnInfo.h"

using namespace std;

class MoveState : public State {
private:
	static MoveState instance;
	MoveState();
public:
	static State * get();

	State * getTransition(TurnInfo& _turnInfo, Agent * agent);
	void onEnter(Agent * agent);
	Action * onUpdate(TurnInfo& _turnInfo, Agent * agent);
	void onExit(Agent * agent);
};