#include "MoveState.h"
#include "../Agent.h"
#include "../MyBotLogic.h"
#include "../Context.h"

#include "WaitState.h"

MoveState MoveState::instance;

MoveState::MoveState()
{
}

State * MoveState::get()
{
	return &instance;
}

State * MoveState::getTransition(TurnInfo & _turnInfo, Agent * agent)
{
	Graph graph = Context::get().getGraph();
	bool found = false;
	for each (Agent * ag in Context::get().getAgents())
	{
		if (ag->getId() != agent->getId() && ag->getPosAtTurn(_turnInfo.turnNb + 1) == agent->getPosAtTurn(_turnInfo.turnNb + 1) && (graph.dist(agent->getPos(), agent->getGoal()) < graph.dist(ag->getPos(), ag->getGoal()) || (graph.dist(agent->getPos(), agent->getGoal()) == graph.dist(ag->getPos(), ag->getGoal()) && agent->getId() > ag->getId()))) {
			found = true;
		}
	}
	if (found || agent->getPos() == agent->getGoal()) {
		return WaitState::get();
	}
	else {
		return nullptr;
	}
}

void MoveState::onEnter(Agent * agent)
{
}

Action * MoveState::onUpdate(TurnInfo& _turnInfo, Agent * agent)
{
	agent->setPos(agent->getPosAtTurn(_turnInfo.turnNb+1));
	return new Move(agent->getId(), agent->getPath()[_turnInfo.turnNb -agent->getWaited() -1]->direction);
}

void MoveState::onExit(Agent * agent)
{
}
