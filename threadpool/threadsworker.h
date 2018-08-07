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

	void stop();
	void terminate();
	
private:
	int m_ThreadNum;
	int m_WorkingNum, m_WaitingNum;
	bool m_State;
	ThreadPool* m_Pool;
	std::thread* m_Threads;
};

