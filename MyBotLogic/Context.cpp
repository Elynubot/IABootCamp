#include "State.h"
#include "Context.h"

#include "windows.h"

Context Context::instance;

Context::Context()
{
}

void Context::start(LevelInfo & _levelInfo)
{
	/*bool debug = true;
	while (debug) {
		Sleep(1000);
	}*/
	graph.init(_levelInfo.rowCount, _levelInfo.colCount, _levelInfo.tiles);
	for each (auto npc in _levelInfo.npcs)
	{
		Agent * ag = new Agent(npc.second.npcID);
		ag->setPos(npc.second.tileID);
		agents.push_back(ag);
	}
	vector<int> goals = graph.getGoalPosition();
	vector<bool> taken;
	for each (int i in goals)
	{
		taken.push_back(false);
	}
	for each (Agent * agent in agents)
	{
		int minDist = -1;
		int minId = -1;
		for (int i = 0; i < goals.size(); i++) {
			if (!taken[i]) {
				if (minDist == -1 || graph.dist(agent->getPos(), goals[i]) < minDist) {
					if (minId != -1) {
						taken[minId] = false;
					}
					minId = i;
					minDist = graph.dist(agent->getPos(), goals[i]);
					taken[minId] = true;
				}
			}
		}
		agent->setGoal(goals[minId]);
	}
	for each (Agent * agent in agents)
	{
		agent->setPath(graph.getPath(agent->getPos(), agent->getGoal()));
	}
}

void Context::update(TurnInfo & _turnInfo, std::vector<Action*>& _actionList)
{
	for each (Agent * agent in agents)
	{
		agent->stateChange(_turnInfo);
	}
	for each (Agent * agent in agents)
	{
		_actionList.push_back(agent->Play(_turnInfo));
	}
}
