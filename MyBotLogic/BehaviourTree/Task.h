#pragma once
#include "../Agent.h"

class Task {
public:
	enum Result {
		SUCCESS,
		FAILURE
	};
	virtual Result execute(Agent *) = 0;
};