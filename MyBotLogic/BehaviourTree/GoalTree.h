#pragma once
#include "Composite.h"
#include "Conditions.h"
#include "Discover.h"
#include "FindObjective.h"
#include "Selector.h"
#include "Sequence.h"
#include "Task.h"

class GoalTree {
public:
	static GoalTree& get() {
		return instance;
	}
	Task::Result execute(Agent*);
private:
	static GoalTree instance;
	GoalTree();
	GoalTree(GoalTree&) = delete;
	GoalTree& operator=(GoalTree&) = delete;
	Selector entry;
};