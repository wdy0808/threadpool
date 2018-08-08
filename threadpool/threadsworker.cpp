#include "stdafx.h"
#include "threadsworker.h"
#include "task.h"
#include "poolcondition.h"

static void threadMain(ThreadsWorker* worker)
{
	worker->work();
}

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

void ThreadsWorker::work()
{
	while (1)
	{
		Task* todo = getWork();
		if (todo == nullptr)
			break;
		todo->run();
		delete todo;
	}
}

int ThreadsWorker::addThreads(int num)
{
	int count = 0;
	for (int i=0; i<m_Threads.size() && count < num; i++)
	{
		if (m_Pool->getThreadState(m_ThreadId[i]) == false)
		{
			m_Threads[i] = std::thread(threadMain, this);
			m_ThreadId[i] = m_Threads[i].get_id();
			m_Pool->setThreadState(m_ThreadId[i], true);
			m_Threads[i].detach();
			count++;
		}
	}
	int offset = static_cast<int>(m_Threads.size()) - count;
	for (int i = count; i < num && m_Threads.size() < m_Pool->getMaxThreadNum(); i++)
	{
		m_Threads.push_back(std::thread(threadMain, this));
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