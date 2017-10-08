#include "Agent.h"
#include "State/MoveState.h"
#include "State/WaitState.h"

Agent::Agent(int agentId)
{
	id = agentId;
	waitState = new WaitState();
	moveState = new MoveState();
	currState = moveState;
}

Action * Agent::Play(TurnInfo& _turnInfo)
{
	return currState->onUpdate(_turnInfo, this);
}

void Agent::stateChange(TurnInfo& _turnInfo)
{
	State::StateType trans;
	trans = currState->getTransition(_turnInfo, this);
	int count = 1000;
	while (trans != State::NO_CHANGE && count>0) {
		currState->onExit(this);
		switch (trans) {
		case State::wait:
			currState = waitState;
			break;
		case State::move:
			currState = moveState;
			break;
		}
		currState->onEnter(this);
		count--;
		trans = currState->getTransition(_turnInfo, this);
	}
}


