// threadpool.cpp: 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "threadpool.h"
#include "testtask.h"
#include "poolcondition.h"

ThreadPool::ThreadPool()
	: ThreadPool(once_all, 5, 5, 100, 0)
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

Task* ThreadPool::getTask()
{
	return m_Controller->getTask();
}

bool ThreadPool::addTask(Task* task, bool ifblock)
{
	if (task == nullptr)
		return false;
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

int main()
{
	ThreadPool pool(5, 1, 100);
	testtask* a = nullptr;
	for (int i = 0; i < 30; i++)
	{
		testtask* task = new testtask(i);
		task->setPriority(i);
		pool.addTask(task, true); if (i == 28) a = task; if (i == 29) task->dependentTo(a);
	}
	//pool.shutdown();
	while (1);
	return 0;
}