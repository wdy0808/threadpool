#pragma once
#include <mutex>

class Task;
class PoolCondition;
struct cmp{
	bool operator() (Task*, Task*);
};

class TaskQueue
{
public:
	TaskQueue(PoolCondition* pool);
	TaskQueue(int, PoolCondition*);
	~TaskQueue();

	bool empty();
	bool full();

	Task* getTask();
	int addTask(Task*);
	int addTask_Block(Task*);

	void stopQueueWork();

	void notifyThreads();

private:
	int m_TaskNum;
	bool stop;
	PoolCondition* m_Pool;
	std::priority_queue<Task*, std::vector<Task*>, cmp> m_TaskQueue;
	std::condition_variable m_QueueNotEmpty;
	std::condition_variable m_QueueNotFull;
	std::mutex m_Mtx;
};

