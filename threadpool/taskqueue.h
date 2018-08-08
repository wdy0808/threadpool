#pragma once
#include <mutex>
class Task;
class PoolCondition;
class TaskQueue
{
public:
	TaskQueue(PoolCondition* pool);
	TaskQueue(int, PoolCondition*);
	~TaskQueue();

	bool empty();
	bool full();

	Task* getTask();
	bool addTask(Task*);
	void addTask_Block(Task*);

	void stopQueueWork();

	void notifyThreads();

private:
	int m_TaskNum;
	bool stop;
	PoolCondition* m_Pool;
	std::queue<Task*> m_TaskQueue;
	std::condition_variable m_QueueNotEmpty;
	std::condition_variable m_QueueNotFull;
	std::mutex m_Mtx;
};

