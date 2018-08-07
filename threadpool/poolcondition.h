#pragma once
class PoolCondition
{
public:
	PoolCondition();
	~PoolCondition();

private:
	std::atomic<int> m_NeedKillThread;
};

