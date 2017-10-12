#include "MoveState.h"
#include "../Agent.h"
#include "../MyBotLogic.h"
#include "../GameManager.h"

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
	Graph graph = GameManager::get().getGraph();
	bool found = false;
	for_each(GameManager::get().getBeginAgent(), GameManager::get().getEndAgent(), [&](Agent * ag) {
		if (ag->getId() != agent->getId() && ag->getPosAtTurn(_turnInfo.turnNb + 1) == agent->getPosAtTurn(_turnInfo.turnNb + 1) && (graph.dist(agent->getPos(), agent->getGoal()) < graph.dist(ag->getPos(), ag->getGoal()) || (graph.dist(agent->getPos(), agent->getGoal()) == graph.dist(ag->getPos(), ag->getGoal()) && agent->getId() > ag->getId()))) {
			found = true;
		}
	});
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
	return new Move(agent->getId(), agent->getPath()[_turnInfo.turnNb -agent->getWaited() -1]->getDirection());
}

void MoveState::onExit(Agent * agent)
{
}
