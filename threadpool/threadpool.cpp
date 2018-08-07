// threadpool.cpp: 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "threadpool.h"
#include "testtask.h"
#include "taskqueue.h"
#include "threadsworker.h"

ThreadPool::ThreadPool()
	: m_TaskQueue(new TaskQueue()), m_Threads(new ThreadsWorker(this))
{
}

ThreadPool::ThreadPool(int threadnum, int tasknum)
	: m_TaskQueue(new TaskQueue(threadnum)), m_Threads(new ThreadsWorker(tasknum, this))
{
}

Task* ThreadPool::getTask()
{
	return m_TaskQueue->getTask();
}

bool ThreadPool::addTask(Task* task, bool ifblock)
{
	if (ifblock)
		m_TaskQueue->addTask_Block(task);
	else
		return m_TaskQueue->addTask(task);
	return true;
}

ThreadPool::~ThreadPool()
{
}

int main()
{
	ThreadPool pool;
	for (int i = 0; i < 30; i++)
	{
		testtask* task = new testtask(i);
		pool.addTask(task, true);
	}
	while (1);
	return 0;
}