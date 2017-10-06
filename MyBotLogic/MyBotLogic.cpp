#include "MyBotLogic.h"

#include "TurnInfo.h"
#include "NPCInfo.h"
#include <chrono>

#include "windows.h"

MyBotLogic::MyBotLogic()
{
	//Write Code Here
}

/*virtual*/ MyBotLogic::~MyBotLogic()
{
	//Write Code Here
}

/*virtual*/ void MyBotLogic::Configure(int argc, char *argv[], const std::string& _logpath)
{
#ifdef BOT_LOGIC_DEBUG
	mLogger.Init(_logpath, "MyBotLogic.log");
#endif

	BOT_LOGIC_LOG(mLogger, "Configure", true);
	
	
	//Write Code Here
}

/*virtual*/ void MyBotLogic::Start()
{
	//Write Code Here
}

/*virtual*/ void MyBotLogic::Init(LevelInfo& _levelInfo)
{
	/*bool debug = true;
	while (debug) {
		Sleep(2000);
	}*/
	graph.init(_levelInfo);
	for each (auto npc in _levelInfo.npcs)
	{
		Agent * ag = new Agent(npc.second.npcID);
		ag->setPos(npc.second.tileID);
		ag->setLogic(this);
		agents.push_back(ag);
	}
	vector<int> goals = graph.getGoalPos();
	vector<bool> taken;
	for each (int i in goals)
	{
		taken.push_back(false);
	}
	for each (Agent * agent in agents)
	{
		//vector<float> dists = graph.getGoalDistances(agent->pos);
		float minDist = -1;
		int minId = -1;
		for (int i = 0; i < goals.size();i++) {
			if (!taken[i]) {
				if (minDist == -1 || graph.dist(agent->getPos(),goals[i]) < minDist) {
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

/*virtual*/ void MyBotLogic::OnGameStarted()
{
	//Write Code Here
}

/*virtual*/ void MyBotLogic::FillActionList(TurnInfo& _turnInfo, std::vector<Action*>& _actionList)
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

/*virtual*/ void MyBotLogic::Exit()
{
	//Write Code Here
}