#pragma once

#include <vector>

#include "LevelInfo.h"
#include "Graph.h"
#include "Agent.h"

class Context {
private:
	static Context instance;

	Graph graph;
	vector<Agent *> agents;

	Context();
	Context(Context&) = delete;
	Context& operator=(Context&) = delete;

public:
	static Context& get() {
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