#include "Selector.h"

Task::Result Selector::execute(Agent * ag)
{
	for each (const auto &child in children)
	{
		if (child->execute(ag) == Task::SUCCESS) {
			return Task::SUCCESS;
		}
	}
	return Task::FAILURE;
}
