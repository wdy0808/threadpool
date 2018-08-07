#include "stdafx.h"
#include "taskqueue.h"
#include "task.h"

TaskQueue::TaskQueue()
	: m_TaskNum(100), m_IfNormal(true)
{
}

TaskQueue::TaskQueue(int num)
	: m_TaskNum(num), m_IfNormal(true)
{
}

TaskQueue::~TaskQueue()
{
}

bool TaskQueue::addTask(Task* task)
{
	if (!m_IfNormal)
		return false;
	std::lock_guard<std::mutex> lock(m_Mtx);
	if (full())
		return false;
	m_TaskQueue.push(task);
	m_QueueNotEmpty.notify_one();
	return true;
}

void TaskQueue::addTask_Block(Task* task)
{
	if (!m_IfNormal)
		return ;
	std::unique_lock<std::mutex> lock(m_Mtx);
	while (full() && m_IfNormal)
		m_QueueNotFull.wait(lock);
	if (!m_IfNormal)
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
	if (!m_IfNormal)
		return nullptr;
	std::unique_lock<std::mutex> lock(m_Mtx);
	while (empty() && m_IfNormal)
		m_QueueNotEmpty.wait(lock);
	if (!m_IfNormal)
		return nullptr;
	Task* task = m_TaskQueue.front();
	m_TaskQueue.pop();
	m_QueueNotFull.notify_one();
	return task;
}

void TaskQueue::setQueueState(bool state)
{
	m_IfNormal = state;
	m_QueueNotEmpty.notify_all();
	m_QueueNotFull.notify_all();
}