#include "stdafx.h"
#include "threadsworker.h"
#include "task.h"
#include "threadpool.h"

static void threadMain(ThreadsWorker* worker)
{
	worker->work();
}

ThreadsWorker::ThreadsWorker(ThreadPool* pool)
	: ThreadsWorker(10, pool)
{
}

ThreadsWorker::ThreadsWorker(int num, ThreadPool* pool)
	: m_ThreadNum(num), m_WorkingNum(0), m_WaitingNum(0), m_Pool(pool)
{
	for (int i = 0; i < m_ThreadNum; i++)
	{
		m_Threads.push_back(std::thread(threadMain, this));
		m_Threads[i].detach();
	}
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
	while (m_Pool->getState() != stop_now)
	{
		Task* todo = getWork();
		if (todo == nullptr)
			break;
		todo->run();
		delete todo;
	}
}
