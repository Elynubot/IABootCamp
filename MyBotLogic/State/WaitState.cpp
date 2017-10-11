#include "WaitState.h"
#include "../Agent.h"
#include "../MyBotLogic.h"
#include "../Context.h"

#include "MoveState.h"

WaitState WaitState::instance;

WaitState::WaitState()
{
}

State * WaitState::get()
{
	return &instance;
}

State * WaitState::getTransition(TurnInfo & _turnInfo, Agent * agent)
{
	Graph graph = Context::get().getGraph();
	bool found = false;
	for (Agent * ag : Context::get().getAgents())
	{
		if (ag->getId() != agent->getId() && ag->getPosAtTurn(_turnInfo.turnNb + 1) == agent->getPosAtTurn(_turnInfo.turnNb + 1) && (graph.dist(agent->getPos(), agent->getGoal()) < graph.dist(ag->getPos(), ag->getGoal()) || (graph.dist(agent->getPos(), agent->getGoal()) == graph.dist(ag->getPos(), ag->getGoal()) && agent->getId() > ag->getId()))) {
			found = true;
		}
	}
	if (found || agent->getPos() == agent->getGoal()) {
		return nullptr;
	}
	else {
		return MoveState::get();
	}
}

void WaitState::onEnter(Agent * agent)
{
}

Action * WaitState::onUpdate(TurnInfo& _turnInfo, Agent * agent)
{
	agent->waitTurn();
	return new Move(agent->getId(), Tile::CENTER);
}

void WaitState::onExit(Agent * agent)
{
}
