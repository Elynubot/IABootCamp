#pragma once

#include <vector>

#include "LevelInfo.h"
#include "Graph.h"
#include "Agent.h"

#ifdef DEBUG
class DuplicateObjectiveException;
#endif // DEBUG
class GameManager {
private:
	static GameManager instance;

	Graph graph;
	vector<Agent> agents;
	vector<int> availableObjectives;

	GameManager();
	GameManager(GameManager&) = delete;
	GameManager& operator=(GameManager&) = delete;

	bool newGoalFound;

public:
	static GameManager& get() {
		return instance;
	}
	vector<Agent>& getAgents() {
		return agents;
	}
	Graph& getGraph() {
		return graph;
	}
	const vector<int>& getAvailableObjectives() {
		return availableObjectives;
	}
	void takeObjective(int id) {
		availableObjectives.erase(std::find(availableObjectives.begin(), availableObjectives.end(), id));
	}
	void returnObjective(int id) {
#ifdef DEBUG
		if (std::find(availableObjectives.begin(), availableObjectives.end(), id) != availableObjectives.end()) {
			throw new DuplicateObjectiveException;
		}
#endif // DEBUG
		availableObjectives.push_back(id);
	}
	void goalFound() {
		newGoalFound = true;
	}
	void start(LevelInfo& _levelInfo);
	void update(TurnInfo& _turnInfo, std::vector<Action*>& _actionList);
};
