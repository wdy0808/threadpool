#pragma once
#include <mutex>
class Task;
class ThreadPool
{
public:
	ThreadPool();
	~ThreadPool();

	Task* getTask();
	void exec();
	void addTask(Task*);

private:
	std::queue<Task* > m_TaskQueue;
	std::vector<std::thread> m_Threads;
	std::mutex m_Mutex;
	std::condition_variable m_Condition;
};

