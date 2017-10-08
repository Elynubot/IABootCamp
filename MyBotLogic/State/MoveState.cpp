#include "MoveState.h"
#include "../Agent.h"
#include "../MyBotLogic.h"
#include "../Context.h"

State::StateType MoveState::getTransition(TurnInfo & _turnInfo, Agent * agent)
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
		return State::wait;
	}
	else {
		return State::NO_CHANGE;
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
