#pragma once
#include "task.h"
class testtask :
	public Task
{
public:
	testtask(int);
	~testtask();

	void run();

private:
	int m_TestNum;
};

