#include "stdafx.h"
#include "dependenttask.h"


DependentTask::DependentTask()
	: m_DependentTask(nullptr)
{
}


DependentTask::~DependentTask()
{
}

void DependentTask::run()
{
	ready(true);
}

bool DependentTask::prepare()
{
	if (m_DependentTask == nullptr)
		return true;
	setFuture(m_DependentTask->getFuture());
	return m_PrepareState.get();
}

void DependentTask::ready(bool state)
{
	m_ReadyState.set_value(state);
}

DependentTask* DependentTask::getDependentTask()
{
	return m_DependentTask;
}

void DependentTask::dependentTo(DependentTask* task)
{
	m_DependentTask = task;
}

std::shared_future<bool>& DependentTask::getFuture()
{
	m_PrepareState = m_ReadyState.get_future().share();
	return m_PrepareState;
}

void DependentTask::setFuture(std::shared_future<bool>& future)
{
	m_PrepareState = future;
}
