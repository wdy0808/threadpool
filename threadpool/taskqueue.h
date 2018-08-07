#pragma once
#include <mutex>
class Task;
class TaskQueue
{
public:
	TaskQueue();
	TaskQueue(int);
	~TaskQueue();

	bool empty();
	bool full();

	Task* getTask();
	bool addTask(Task*);
	void addTask_Block(Task*);

private:
	int m_TaskNum;
	std::queue<Task*> m_TaskQueue;
	std::condition_variable m_QueueNotEmpty;
	std::condition_variable m_QueueNotFull;
	std::mutex m_Mtx;
};

