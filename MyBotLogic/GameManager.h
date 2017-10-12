#pragma once

#include <vector>

#include "LevelInfo.h"
#include "Graph.h"
#include "Agent.h"

class GameManager {
private:
	static GameManager instance;

	Graph graph;
	vector<Agent *> agents;

	GameManager();
	GameManager(GameManager&) = delete;
	GameManager& operator=(GameManager&) = delete;

public:
	static GameManager& get() {
		return instance;
	}

	vector<Agent *>& getAgents() {
		return agents;
	}
	Graph& getGraph() {
		return graph;
	}
	void start(LevelInfo& _levelInfo);
	void update(TurnInfo& _turnInfo, std::vector<Action*>& _actionList);
};