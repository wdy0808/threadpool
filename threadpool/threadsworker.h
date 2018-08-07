#pragma once
class Task;
class ThreadPool;
class ThreadsWorker
{
public:
	ThreadsWorker(ThreadPool* pool);
	ThreadsWorker(int, ThreadPool*);
	~ThreadsWorker();

	void work();
	Task* getWork();
	
private:
	int m_ThreadNum;
	int m_WorkingNum, m_WaitingNum;
	ThreadPool* m_Pool;
	std::vector<std::thread> m_Threads;
};

