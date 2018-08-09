#pragma once
class Task;
class PoolCondition;
enum ThreadIncreaseWay;
enum ThreadPoolState;
class ThreadPool
{
public:
	ThreadPool(ThreadIncreaseWay way, int start, int maxthread, int maxtask, int addnum = 0);
	ThreadPool(int threadnum, int starthread, int tasknum = 100);
	ThreadPool();
	~ThreadPool();

	Task* getTask();
	bool addTask(Task* task, bool ifblock);

	void stop();
	void shutdown();

	int getWorkedThreadsNum();

	void setIncreaseMode(ThreadIncreaseWay way, int num);

	bool cancelTask(Task*);

private:
	PoolCondition* m_Controller;
};

