// threadpool_test.cpp: 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "testtask.h"

int main()
{
	ThreadPool pool(5, 1, 100);
	testtask* a = nullptr;
	for (int i = 0; i < 30; i++)
	{
		testtask* task = new testtask(i);
		task->setPriority(i);
		pool.addTask(task, true); if (i == 28) a = task; if (i == 29) task->dependentTo(a);
	}
	//pool.shutdown();
	while (1);
	return 0;
}

