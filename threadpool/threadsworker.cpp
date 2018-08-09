#include "stdafx.h"
#include "threadsworker.h"
#include "task.h"
#include "dependenttask.h"
#include "poolcondition.h"

ThreadsWorker::ThreadsWorker(PoolCondition* pool)
	: m_Pool(pool)
{
	m_ThreadId.assign(m_Pool->getMaxThreadNum(), std::thread::id());
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
}

int ThreadsWorker::addThreads(int num)
{
	int count = 0;
	for (int i=0; i<m_Threads.size() && count < num; i++)
	{
		if (m_Pool->getThreadState(m_ThreadId[i]) == false)
		{
			m_Threads.push_back(std::thread([this]() {work(); }));
			m_ThreadId[i] = m_Threads[i].get_id();
			m_Pool->setThreadState(m_ThreadId[i], true);
			m_Threads[i].detach();
			count++;
		}
	}
	int offset = static_cast<int>(m_Threads.size()) - count;
	for (int i = count; i < num && m_Threads.size() < m_Pool->getMaxThreadNum(); i++)
	{
		m_Threads.push_back(std::thread([this]() {work(); }));
		m_ThreadId[i + offset] = m_Threads[i + offset].get_id();
		m_Pool->setThreadState(m_Threads[i + offset].get_id(), true);
		m_Threads[i + offset].detach();
		count++;
	}
	return count;
}

std::thread::id ThreadsWorker::getThreadId(int index)
{
	return m_ThreadId[index];
}