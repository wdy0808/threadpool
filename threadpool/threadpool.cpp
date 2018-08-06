// threadpool.cpp: 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "threadpool.h"
#include "task.h"
#include "testtask.h"

const int ThreadNum = 10;
void exec1(ThreadPool*);
ThreadPool::ThreadPool()
{
	for (auto i = 0; i < ThreadNum; i++)
	{
		m_Threads.push_back(std::thread(exec1, this));
		m_Threads[i].detach();
	}
}

Task* ThreadPool::getTask()
{
	if (m_TaskQueue.empty())
		return nullptr;
	printf("%d %d\n", m_TaskQueue.empty(), std::this_thread::get_id());
	Task* tem = m_TaskQueue.front();
	m_TaskQueue.pop(); printf("delete %d\n", std::this_thread::get_id());
	return tem;
}

void ThreadPool::exec()
{
	while (1)
	{
		Task* todo = nullptr;
		{
			printf("%d try to get work\n", std::this_thread::get_id());
			std::unique_lock<std::mutex> lock(m_Mutex);
			todo = getTask();
			while (todo == nullptr)
			{
				m_Condition.wait(lock);
				todo = getTask();
			}
		}
		printf("%d get work\n", std::this_thread::get_id);
		todo->run();
	}
}

void ThreadPool::addTask(Task* task)
{
	m_TaskQueue.push(task);
	m_Condition.notify_one();
}

ThreadPool::~ThreadPool()
{
}

void exec1(ThreadPool* pool)
{
	pool->exec();
}

int main()
{
	ThreadPool pool;
	for (int i = 0; i < 30; i++)
	{
		testtask* task = new testtask(i);
		pool.addTask(task);
	}
	while (1);
	return 0;
}