#include "Sequence.h"

Task::Result Sequence::execute(Agent * ag)
{
	for each (const auto &child in children)
	{
		if (child->execute(ag) == Task::FAILURE) {
			return Task::FAILURE;
		}
	}
	return Task::SUCCESS;
}
