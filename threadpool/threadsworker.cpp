#include "stdafx.h"
#include "threadsworker.h"
#include "task.h"
#include "dependenttask.h"
#include "poolcondition.h"

ThreadsWorker::ThreadsWorker(PoolCondition* pool)
	: m_Pool(pool)
{
}

ThreadsWorker::~ThreadsWorker()
{
}

Task* ThreadsWorker::getWork()
{
	return m_Pool->getTask();
}

bool ThreadsWorker::readyToWork(Task* task)
{
	DependentTask* dtask = dynamic_cast<DependentTask*>(task);
	if (dtask == NULL || dtask->getDependentTask() == nullptr)
		return true;
	try {
		return dtask->prepare();
	}catch (std::exception& e) {
		printf("[exception caught: %s]\n有任务依赖任务未表明是否被依赖任务可执行!任务放弃执行\n", e.what());
		return false;
	}
}

void ThreadsWorker::work()
{
	while (1)
	{
		Task* todo = getWork();
		if (todo == nullptr)
			break;
		if (readyToWork(todo))
			todo->run();
		delete todo;
		todo = nullptr;
	}
	m_Pool->release();
}

int ThreadsWorker::addThreads(int num)
{
	int count = (num + m_Pool->getWorkedThreadsNum() <= m_Pool->getMaxThreadNum()) ? num :  m_Pool->getMaxThreadNum() - m_Pool->getWorkedThreadsNum();
	for (int i = 0; i < count; i++)
		std::thread([this]() {work(); }).detach();
	return count;
}