#pragma once
class testtask :
	public DependentTask
{
public:
	testtask(int);
	~testtask();

	void run();

private:
	int m_TestNum;
};

