#include "stdafx.h"
#include "taskqueue.h"
#include "task.h"

TaskQueue::TaskQueue()
	: m_TaskNum(100)
{
}

TaskQueue::TaskQueue(int num)
	: m_TaskNum(num)
{
}

TaskQueue::~TaskQueue()
{
}

bool TaskQueue::addTask(Task* task)
{
	std::lock_guard<std::mutex> lock(m_Mtx);
	if (full())
		return false;
	m_TaskQueue.push(task);
	m_QueueNotEmpty.notify_one();
	return true;
}

void TaskQueue::addTask_Block(Task* task)
{
	std::unique_lock<std::mutex> lock(m_Mtx);
	while (full())
		m_QueueNotFull.wait(lock);
	m_TaskQueue.push(task);
	m_QueueNotEmpty.notify_one();
}

bool TaskQueue::empty()
{
	return m_TaskQueue.empty();
}

bool TaskQueue::full()
{
	return m_TaskQueue.size() == m_TaskNum;
}

Task* TaskQueue::getTask()
{
	std::unique_lock<std::mutex> lock(m_Mtx);
	while (empty())
		m_QueueNotEmpty.wait(lock);
	Task* task = m_TaskQueue.front();
	m_TaskQueue.pop();
	m_QueueNotFull.notify_one();
	return task;
}