#pragma once
class PoolCondition;
class Timer
{
public:
	Timer(PoolCondition*);
	~Timer();

	void startTimer(int, std::function<void()>);
	void expire();

private:
	std::atomic<bool> m_Expire;
	PoolCondition* m_Pool;
};

