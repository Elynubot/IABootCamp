#pragma once

#include "LevelInfo.h"
#include "Graph.h"
#include "Agent.h"
#include "Utility.h"
#include <vector>

class GameManager {
private:
	static GameManager instance;

	Graph graph;

	class AgentPtrComparison {
	private:
		bool reverse;
	public:
		AgentPtrComparison(const bool& revparam = false) : reverse{ revparam } {}
		bool operator() (Agent* _lagent, Agent* _ragent) const
		{
			if (reverse)
				return (_lagent->getPath().size() > _ragent->getPath().size());
			else
				return (_lagent->getPath().size() < _ragent->getPath().size());
		}
	};
	MyPriorityQueue<Agent*, std::vector<Agent*>, GameManager::AgentPtrComparison> agents;

	GameManager();
	GameManager(GameManager&) = delete;
	GameManager& operator=(GameManager&) = delete;
	~GameManager();

public:
	static GameManager& get() {
		return instance;
	}

	std::vector<Agent*>::iterator getBeginAgent() noexcept {
		return agents.begin();
	}
	std::vector<Agent*>::iterator getEndAgent() noexcept {
		return agents.end();
	}

	Graph& getGraph() {
		return graph;
	}
	void start(LevelInfo& _levelInfo);
	void update(TurnInfo& _turnInfo, std::vector<Action*>& _actionList);
};