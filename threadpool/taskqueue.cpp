#include "stdafx.h"
#include "taskqueue.h"
#include "task.h"
#include "threadpool.h"

TaskQueue::TaskQueue(ThreadPool* pool)
	: m_TaskNum(100), m_Pool(pool)
{
}

TaskQueue::TaskQueue(int num, ThreadPool* pool)
	: m_TaskNum(num), m_Pool(pool)
{
}

TaskQueue::~TaskQueue()
{
}

bool TaskQueue::addTask(Task* task)
{
	std::lock_guard<std::mutex> lock(m_Mtx);
	if (full() || m_Pool->getState() != normal)
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
	if (m_Pool->getState() != normal)
		return;
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
	while (empty() && m_Pool->getState() == normal)
		m_QueueNotEmpty.wait(lock);
	if (m_Pool->getState() == stop_now || empty())
		return nullptr;
	Task* task = m_TaskQueue.front();
	m_TaskQueue.pop();
	m_QueueNotFull.notify_one();
	return task;
}

void TaskQueue::stopQueueWork()
{
	std::unique_lock<std::mutex> lock(m_Mtx);
	if (m_Pool->getState() == stop_now)
	{
		while (!m_TaskQueue.empty())
			m_TaskQueue.pop();
	}
	m_QueueNotEmpty.notify_all();
	m_QueueNotFull.notify_all();
}