#pragma once
#include <mutex>
class Task;
class TaskQueue;
class ThreadsWorker;
class ThreadPool
{
public:
	ThreadPool();
	ThreadPool(int threadnum, int tasknum = 100);
	~ThreadPool();

	Task* getTask();
	bool addTask(Task* task, bool ifblock);

private:
	TaskQueue * m_TaskQueue;
	ThreadsWorker * m_Threads;
};

