#pragma once
#include "stdafx.h"
#include "config.h"
#include "task.h"
#include "dependenttask.h"
class PoolCondition;
class ThreadPool
{
public:
	ThreadPool(ThreadIncreaseWay way, int start, int maxthread, int maxtask, int addnum = 1);
	ThreadPool(int threadnum, int starthread, int tasknum = 100);
	ThreadPool();
	~ThreadPool();

	bool addTask(Task* task, bool ifblock);

	void stop();
	void shutdown();

	int getWorkedThreadsNum();

	void setIncreaseMode(ThreadIncreaseWay way, int num = 1);

	bool cancelTask(Task*);

private:
	PoolCondition* m_Controller;
};

