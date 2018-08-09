#pragma once
#include "task.h"
#include <future>
class DependentTask :
	public Task
{
public:
	DependentTask();
	virtual ~DependentTask();

	virtual void run();

	bool prepare();
	void ready(bool);

	DependentTask* getDependentTask();
	void dependentTo(DependentTask*);

	std::shared_future<bool>& getFuture();
	void setFuture(std::shared_future<bool>&);

private:
	std::promise<bool> m_ReadyState;
	std::shared_future<bool> m_PrepareState;
	DependentTask* m_DependentTask;
};

