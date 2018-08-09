#pragma once
class Task;
class PoolCondition;
class ThreadsWorker
{
public:
	ThreadsWorker(PoolCondition* pool);
	~ThreadsWorker();

	bool readyToWork(Task*);
	void work();
	Task* getWork();

	int addThreads(int);

	std::thread::id getThreadId(int);
	
private:
	PoolCondition* m_Pool;
	std::vector<std::thread> m_Threads;
	std::vector<std::thread::id> m_ThreadId;
};

