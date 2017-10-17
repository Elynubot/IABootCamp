#include "State.h"
#include "GameManager.h"

#include "windows.h"

GameManager GameManager::instance;

GameManager::GameManager()
{
}

GameManager::~GameManager()
{
	for_each(getBeginAgent(), getEndAgent(), [](Agent* ag) { delete(ag); });
}


void GameManager::start(LevelInfo & _levelInfo)
{
	graph.init(_levelInfo.rowCount, _levelInfo.colCount, _levelInfo.tiles, _levelInfo.objects);
	for (const auto& npc : _levelInfo.npcs)
	{
		Agent * ag = new Agent((int)npc.second.npcID );
		ag->setPos(npc.second.tileID);
		agents.push_back(ag);
	}
	/*vector<int> goals = graph.getGoalPosition();
	vector<bool> taken;
	for (int i : goals)
	{
		taken.push_back(false);
	}
	for (Agent * agent : agents)
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
	for (Agent * agent : agents)
	{
		agent->setPath(graph.getPath(agent->getPos(), agent->getGoal()));
	}*/
}

void GameManager::update(TurnInfo & _turnInfo, std::vector<Action*>& _actionList)
{
	graph.update(_turnInfo.tiles,_turnInfo.objects);
	for (Agent * agent : agents) {
		agent->checkPath();
	}
	graph.popInvalidConnectors();

	sort(agents.begin(), agents.end(), AgentPtrComparison());

	for (Agent * agent : agents)
	{
		agent->makeDecisions();
	}
	for (Agent * agent : agents)
	{
		agent->stateChange(_turnInfo);
	}
	for (Agent * agent : agents)
	{
		_actionList.push_back(agent->play(_turnInfo));
	}
	newGoalFound = false;
}
