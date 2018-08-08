#pragma once
enum ThreadIncreaseWay{once_all, increase_linear, increase_index};
class TaskQueue;
class ThreadsWorker;
class PoolCondition
{
public:
	PoolCondition(ThreadIncreaseWay way, int start, int add);
	~PoolCondition();

	void activiteMoreThread();
	void releaseOneThread();

	int getWorkedThreadsNum();

	void setIncreaseMode(ThreadIncreaseWay way, int num);
	void setDecreaseMode(bool);

private:
	std::map<std::thread::id, bool> m_ThreadCondition;
	int m_StartNum, m_IncreaseNum, m_WorkedNum;
	ThreadIncreaseWay m_Way;
	std::atomic<bool> m_KillThread, m_Decrease;
	TaskQueue* m_Task;
	ThreadsWorker* m_Thread;
};

