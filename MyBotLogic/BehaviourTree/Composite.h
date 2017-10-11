#pragma once
#include "Task.h"
#include <memory>

class Composite : public Task {
public:
	void addChild(std::unique_ptr<Task>);

protected:
	std::vector<std::unique_ptr<Task>> children;
};