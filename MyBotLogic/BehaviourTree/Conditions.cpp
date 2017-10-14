#include "Conditions.h"

Task::Result IsGoalReached::execute(Agent * ag)
{
	if (ag->getPos() == ag->getGoal()) {
		return this->SUCCESS;
	}
	else {
		return this->FAILURE;
	}
}

Task::Result IsPathValid::execute(Agent * ag)
{
	if (ag->getPathValid()) {
		return this->SUCCESS;
	}
	else {
		return this->FAILURE;
	}
}

Task::Result IsNewPathNeeded::execute(Agent *)
{
	return Result();
}
