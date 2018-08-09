#include "stdafx.h"
#include "taskqueue.h"
#include "task.h"
#include "poolcondition.h"

bool cmp::operator() (Task* a, Task* b)
{
	if (a->getPriority() != b->getPriority())
		return a->getPriority() < b->getPriority();
	return a->getId() > b->getId();
}

TaskQueue::TaskQueue(PoolCondition* pool)
	: m_TaskNum(100), m_Pool(pool), stop(false)
{
}

TaskQueue::TaskQueue(int num, PoolCondition* pool)
	: m_TaskNum(num), m_Pool(pool), stop(false)
{
}

TaskQueue::~TaskQueue()
{
}

int TaskQueue::addTask(Task* task)
{
	std::lock_guard<std::mutex> lock(m_Mtx);
	if (full() || stop)
		return 0;
	m_TaskQueue.push(task);
	m_QueueNotEmpty.notify_one();
	if (m_TaskQueue.size() > m_Pool->getWorkedThreadsNum())
		m_Pool->activiteMoreThread(false);
	return task->getId();
}

int TaskQueue::addTask_Block(Task* task)
{
	std::unique_lock<std::mutex> lock(m_Mtx);
	while (full())
		m_QueueNotFull.wait(lock);
	if (stop)
		return 0;
	m_TaskQueue.push(task);
	m_QueueNotEmpty.notify_one();
	if (m_TaskQueue.size() > m_Pool->getWorkedThreadsNum())
		m_Pool->activiteMoreThread(false);
	return task->getId();
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
	while (empty() && !stop && (m_Pool->getState() == pausing || m_Pool->getThreadState(std::this_thread::get_id())))
		m_QueueNotEmpty.wait(lock);
	if (m_Pool->getState() == stop_now || empty() || !m_Pool->getThreadState(std::this_thread::get_id()))
		return nullptr;
	Task* task = m_TaskQueue.top();
	m_TaskQueue.pop();
	m_QueueNotFull.notify_one();
	return task;
}

void TaskQueue::stopQueueWork()
{
	std::unique_lock<std::mutex> lock(m_Mtx);
	stop = true;
	if (m_Pool->getState() == stop_now)
	{
		while (!m_TaskQueue.empty())
		{
			Task* tem = m_TaskQueue.top();
			m_TaskQueue.pop();
			delete tem;
		}
	}
	m_QueueNotEmpty.notify_all();
	m_QueueNotFull.notify_all();
}

void TaskQueue::notifyThreads()
{
	m_QueueNotEmpty.notify_all();
}