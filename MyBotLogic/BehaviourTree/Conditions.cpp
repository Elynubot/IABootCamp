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
	for(auto& connector : ag->getPath()) {
		if (connector = nullptr) {
			return this->FAILURE;
		}
	}
	return this->SUCCESS;
}

Task::Result IsNewPathNeeded::execute(Agent *)
{
	return Result();
}
