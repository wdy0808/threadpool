#pragma once
enum ThreadIncreaseWay{once_all, increase_linear, increase_index};
enum ThreadPoolState {normal, pausing, stop, stop_now};
class TaskQueue;
class ThreadsWorker;
class Task;
class PoolCondition
{
public:
	PoolCondition(ThreadIncreaseWay way, int start, int max, int maxtask, int add, bool d, double minus);
	~PoolCondition();

	void activiteMoreThread(bool initialize);
	void releaseThread();

	int getWorkedThreadsNum();
	int getMaxThreadNum();

	void setIncreaseMode(ThreadIncreaseWay way, int num);
	void setDecreaseMode(bool);

	Task* getTask();
	int addTask(Task*, bool);

	bool getThreadState(std::thread::id);
	void setThreadState(std::thread::id, bool);

	ThreadPoolState getState();
	void setState(ThreadPoolState);

	void stop();
	void shutdown();

	bool activeAll();
	bool activeOne();

private:
	std::map<std::thread::id, bool> m_ThreadCondition;
	int m_StartNum, m_IncreaseNum, m_WorkedNum, m_MaxThreadNum;
	ThreadIncreaseWay m_Way;
	std::atomic<bool> m_Decrease;
	double m_DecreaseRate;
	TaskQueue* m_Task;
	ThreadsWorker* m_Thread;
	ThreadPoolState m_State;
};