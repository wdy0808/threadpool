// threadpool.cpp: 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "threadpool.h"
#include "testtask.h"
#include "taskqueue.h"
#include "threadsworker.h"

ThreadPool::ThreadPool()
	: m_TaskQueue(new TaskQueue(this)), m_Threads(new ThreadsWorker(this)), m_State(normal)
{
}

ThreadPool::ThreadPool(int threadnum, int tasknum)
	: m_TaskQueue(new TaskQueue(threadnum, this)), m_Threads(new ThreadsWorker(tasknum, this)), m_State(normal)
{
}

ThreadPool::~ThreadPool()
{
	if (m_TaskQueue != nullptr)
	{
		delete m_TaskQueue;
		m_TaskQueue = nullptr;
	}
	if (m_Threads != nullptr)
	{
		delete m_Threads;
		m_Threads = nullptr;
	}
}

Task* ThreadPool::getTask()
{
	if (m_State == stop_now)
		return nullptr;
	return m_TaskQueue->getTask();
}

bool ThreadPool::addTask(Task* task, bool ifblock)
{
	if (m_State != normal)
		return false;
	if (ifblock)
		m_TaskQueue->addTask_Block(task);
	else
		return m_TaskQueue->addTask(task);
	return true;
}

void ThreadPool::stop()
{
	if (m_State == normal)
	{
		m_State = ThreadPoolState::stop;
		m_TaskQueue->stopQueueWork();
	}
}

void ThreadPool::shutdown()
{
	if (m_State == normal)
	{
		m_State = ThreadPoolState::stop_now;
		m_TaskQueue->stopQueueWork();
	}
}

ThreadPoolState ThreadPool::getState()
{
	return m_State;
}

int main()
{
	ThreadPool pool;
	for (int i = 0; i < 30; i++)
	{
		testtask* task = new testtask(i);
		pool.addTask(task, true);
	}
	//pool.shutdown();
	while (1);
	return 0;
}