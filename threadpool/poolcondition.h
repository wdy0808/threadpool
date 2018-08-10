#pragma once
#include "config.h"
class TaskQueue;
class ThreadsWorker;
class Task;
class PoolCondition
{
public:
	PoolCondition(ThreadIncreaseWay way, int start, int max, int maxtask, int add);
	~PoolCondition();

	void activiteMoreThread(bool initialize);

	int getWorkedThreadsNum();
	int getMaxThreadNum();

	void setIncreaseMode(ThreadIncreaseWay way, int num = 0);

	Task* getTask();
	bool addTask(Task*, bool);

	void release();

	ThreadPoolState getState();

	void stop();
	void shutdown();

	bool cancelTask(Task*);

private:
	int m_StartNum, m_IncreaseNum, m_MaxThreadNum;
	std::atomic<int> m_WorkedNum;
	ThreadIncreaseWay m_Way;
	TaskQueue* m_Task;
	ThreadsWorker* m_Thread;
	ThreadPoolState m_State;
};