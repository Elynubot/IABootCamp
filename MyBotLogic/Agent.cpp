#include "Agent.h"
#include "MoveState.h"
#include "WaitState.h"

Agent::Agent(int agentId)
{
	id = agentId;
	waitState = new WaitState();
	waitState->setAgent(this);
	moveState = new MoveState();
	moveState->setAgent(this);
	currState = moveState;
}

Action * Agent::Play(TurnInfo& _turnInfo)
{
	return currState->onUpdate(_turnInfo);
}

void Agent::stateChange(TurnInfo& _turnInfo)
{
	State::StateType trans;
	trans = currState->getTransition(_turnInfo);
	int count = 1000;
	while (trans != State::NO_CHANGE && count>0) {
		currState->onExit();
		switch (trans) {
		case State::wait:
			currState = waitState;
			break;
		case State::move:
			currState = moveState;
			break;
		}
		currState->onEnter();
		count--;
		trans = currState->getTransition(_turnInfo);
	}
}


