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
	
private:
	PoolCondition* m_Pool;
};

