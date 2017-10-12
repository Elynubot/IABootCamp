#include "Agent.h"
#include "State/MoveState.h"
#include "State/WaitState.h"

Agent::Agent(int agentId)
{
	id = agentId;
	currState = MoveState::get();
}

void Agent::makeDecisions()
{
	decision.execute(this);
}

Action * Agent::play(TurnInfo& _turnInfo)
{
	return currState->onUpdate(_turnInfo, this);
}

void Agent::stateChange(TurnInfo& _turnInfo)
{
	State * trans;
	trans = currState->getTransition(_turnInfo, this);
	int count = 1000;
	while (trans != nullptr && count>0) {
		currState->onExit(this);
		currState = trans;
		currState->onEnter(this);
		count--;
		trans = currState->getTransition(_turnInfo, this);
	}
}


