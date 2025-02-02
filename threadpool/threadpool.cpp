#include "stdafx.h"
#include "threadpool.h"
#include "poolcondition.h"

ThreadPool::ThreadPool()
	: ThreadPool(increase_linear, 5, 1, 100, 1)
{
}

ThreadPool::ThreadPool(int threadnum, int starthread, int tasknum)
	: ThreadPool(increase_linear, starthread, threadnum, tasknum, ceil(starthread*1.0/2) )
{
}

ThreadPool::ThreadPool(ThreadIncreaseWay way, int startthread, int maxthread, int maxtask, int addnum)
	: m_Controller(new PoolCondition(way, startthread, maxthread, maxtask, addnum))
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

bool ThreadPool::cancelTask(Task* task)
{
	return m_Controller->cancelTask(task);
}