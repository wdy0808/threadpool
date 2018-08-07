#pragma once
#include <mutex>
class Task;
class ThreadPool;
class TaskQueue
{
public:
	TaskQueue(ThreadPool* pool);
	TaskQueue(int, ThreadPool*);
	~TaskQueue();

	bool empty();
	bool full();

	Task* getTask();
	bool addTask(Task*);
	void addTask_Block(Task*);

	void stopQueueWork();

private:
	int m_TaskNum;
	ThreadPool* m_Pool;
	std::queue<Task*> m_TaskQueue;
	std::condition_variable m_QueueNotEmpty;
	std::condition_variable m_QueueNotFull;
	std::mutex m_Mtx;
};

