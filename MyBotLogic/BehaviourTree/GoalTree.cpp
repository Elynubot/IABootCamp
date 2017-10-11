#include "GoalTree.h"

GoalTree GoalTree::instance;

Task::Result GoalTree::execute(Agent * ag)
{
	entry.execute(ag);
}

GoalTree::GoalTree()
{
	unique_ptr<Selector> objSelector = std::make_unique<Selector>(Selector{});
	unique_ptr<Discover> discover = std::make_unique<Discover>(Discover{});
	unique_ptr<IsGoalReached> goalReached = std::make_unique<IsGoalReached>(IsGoalReached{});
	unique_ptr<Sequence> nObjSequence = std::make_unique<Sequence>(Sequence{});
	unique_ptr<IsPathValid> pathValid = std::make_unique<IsPathValid>(IsPathValid{});
	unique_ptr<IsNewPathNeeded> newPathNeeded = std::make_unique<IsNewPathNeeded>(IsNewPathNeeded{});
	unique_ptr<FindObjective> findObj = std::make_unique<FindObjective>(FindObjective{});
	entry.addChild(std::move(objSelector));
	entry.addChild(std::move(discover));
	objSelector->addChild(std::move(discover));
	objSelector->addChild(std::move(nObjSequence));
	objSelector->addChild(std::move(pathValid));
	nObjSequence->addChild(std::move(newPathNeeded));
	nObjSequence->addChild(std::move(findObj));
}
