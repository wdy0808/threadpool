// threadpool.cpp: 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "threadpool.h"
#include "task.h"

const int ThreadNum = 10;

ThreadPool::ThreadPool()
{
	for (auto i = m_Threads.begin(); i < m_Threads.end(); i++)
	{
		(*i) = std::thread(this->exec);
		i->join();
	}
}

Task* ThreadPool::getTask()
{
	if (m_TaskQueue.empty())
		return nullptr;
	Task* tem = m_TaskQueue.front();
	m_TaskQueue.pop();
	return tem;
}

void ThreadPool::exec()
{
	while (1)
	{
		Task* todo;
		{
			std::unique_lock<std::mutex> lock(m_Mutex);
			while (getTask() == nullptr)
				m_Condition.wait(lock);
			todo = getTask();
		}
		todo->run();
	}
}

void ThreadPool::addTask(Task* task)
{
	m_TaskQueue.push(task);
	m_Condition.notify_one();
}

ThreadPool::~ThreadPool()
{
}

ThreadPool& ThreadPool::getThreadPool()
{
	
	static ThreadPool threadPool;
	return threadPool;
}