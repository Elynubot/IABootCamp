#include "WaitState.h"
#include "Agent.h"
#include "MyBotLogic.h"

State::StateType WaitState::getTransition(TurnInfo & _turnInfo)
{
	Graph * graph = agent->getLogic()->getGraph();
	bool found = false;
	for each (Agent * ag in *agent->getLogic()->getAgents())
	{
		if (ag->getId() != agent->getId() && ag->getPosAtTurn(_turnInfo.turnNb + 1) == agent->getPosAtTurn(_turnInfo.turnNb + 1) && (graph->dist(agent->getPos(), agent->getGoal()) < graph->dist(ag->getPos(), ag->getGoal()) || (graph->dist(agent->getPos(), agent->getGoal()) == graph->dist(ag->getPos(), ag->getGoal()) && agent->getId() > ag->getId()))) {
			found = true;
		}
	}
	if (found || agent->getPos() == agent->getGoal()) {
		return State::NO_CHANGE;
	}
	else {
		return State::move;
	}
}

void WaitState::onEnter()
{
}

Action * WaitState::onUpdate(TurnInfo& _turnInfo)
{
	agent->waitTurn();
	return new Move(agent->getId(), Tile::CENTER);
}

void WaitState::onExit()
{
}
