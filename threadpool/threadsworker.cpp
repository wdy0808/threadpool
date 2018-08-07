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
	: m_ThreadNum(num), m_WorkingNum(0), m_WaitingNum(0), m_Pool(pool), m_State(true)
{
	m_Threads = new std::thread[m_ThreadNum];
	for (int i = 0; i < m_ThreadNum; i++)
	{
		m_Threads[i] = std::thread(threadMain, this);
		m_Threads[i].detach();
	}
}

ThreadsWorker::~ThreadsWorker()
{
	if (m_Threads != nullptr)
	{
		delete[] m_Threads;
		m_Threads = nullptr;
	}
}

Task* ThreadsWorker::getWork()
{
	return m_Pool->getTask();
}

void ThreadsWorker::work()
{
	while (m_State)
	{
		Task* todo = getWork();
		if (todo == nullptr)
			break;
		todo->run();
	}
}

void ThreadsWorker::stop()
{
	m_State = false;
}

void ThreadsWorker::terminate()
{
	delete[] m_Threads;
	m_Threads = nullptr;
}
