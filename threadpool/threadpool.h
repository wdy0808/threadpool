#pragma once
#include <mutex>

enum ThreadPoolState {normal, stop, stop_now};

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

	void stop();
	void shutdown();

	ThreadPoolState getState();

private:
	TaskQueue * m_TaskQueue;
	ThreadsWorker * m_Threads;
	ThreadPoolState m_State;
};

