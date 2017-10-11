#include "Composite.h"

void Composite::addChild(unique_ptr<Task> child)
{
	children.push_back(std::move(child));
}
