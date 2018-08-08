// threadpool.cpp: 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "threadpool.h"
#include "testtask.h"
#include "poolcondition.h"

ThreadPool::ThreadPool()
	: ThreadPool(once_all, 10, 10, 100, 0, false, 1)
{
}

ThreadPool::ThreadPool(int threadnum, int starthread, int tasknum)
	: ThreadPool(increase_linear, starthread, threadnum, tasknum, starthread/2 , false, 1)
{
}

ThreadPool::ThreadPool(ThreadIncreaseWay way, int start, int maxthread, int maxtask, int addnum, bool decrease, double rate)
	: m_Controller(new PoolCondition(way, start, maxthread, maxtask, addnum, decrease, rate))
{
}

ThreadPool::~ThreadPool()
{
	if (m_Controller == nullptr)
	{
		delete m_Controller;
		m_Controller = nullptr;
	}
}

Task* ThreadPool::getTask()
{
	return m_Controller->getTask();
}

bool ThreadPool::addTask(Task* task, bool ifblock)
{
	return m_Controller->addTask(task, ifblock);
}

void ThreadPool::stop()
{
	m_Controller->stop();
}

void ThreadPool::shutdown()
{
	m_Controller->shutdown();
}

int ThreadPool::getWorkedThreadsNum()
{
	return m_Controller->getWorkedThreadsNum();
}

void ThreadPool::setIncreaseMode(ThreadIncreaseWay way, int num)
{
	m_Controller->setIncreaseMode(way, num);
}
void ThreadPool::setDecreaseMode(bool mode)
{
	m_Controller->setDecreaseMode(mode);
}

int main()
{
	ThreadPool pool(10, 5, 100) ;
	for (int i = 0; i < 11; i++)
	{
		testtask* task = new testtask(i);
		pool.addTask(task, true);
	}
	//pool.shutdown();
	while (1);
	return 0;
}