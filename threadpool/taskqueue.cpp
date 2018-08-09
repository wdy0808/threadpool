#include "stdafx.h"
#include "taskqueue.h"
#include "task.h"
#include "poolcondition.h"

bool cmp::operator() (Task* a, Task* b)
{
	return a->getPriority() < b->getPriority();
}

TaskQueue::TaskQueue(PoolCondition* pool)
	: TaskQueue(100, pool)
{
}

TaskQueue::TaskQueue(int num, PoolCondition* pool)
	: m_TaskNum(num), m_Pool(pool), stop(false)
{
}

TaskQueue::~TaskQueue()
{
}

bool TaskQueue::addTask(Task* task)
{
	std::lock_guard<std::mutex> lock(m_Mtx);
	if (full() || stop)
		return false;
	m_TaskQueue.push(task);
	m_QueueNotEmpty.notify_one();
	if (m_TaskQueue.size() > m_Pool->getWorkedThreadsNum())
		m_Pool->activiteMoreThread(false);
	return true;
}

void TaskQueue::addTask_Block(Task* task)
{
	std::unique_lock<std::mutex> lock(m_Mtx);
	while (full())
		m_QueueNotFull.wait(lock);
	if (stop)
		return ;
	m_TaskQueue.push(task);
	m_QueueNotEmpty.notify_one();
	if (m_TaskQueue.size() > m_Pool->getWorkedThreadsNum())
		m_Pool->activiteMoreThread(false);
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
	clock_t begin_wait = clock();
	while (empty() && !stop && (m_Pool->getState() == pausing || m_Pool->getThreadState(std::this_thread::get_id())))
	{
		m_QueueNotEmpty.wait_for(lock, std::chrono::seconds(120));
		clock_t end_wait = clock();
		double wait_time = static_cast<double>(end_wait - begin_wait) / CLOCKS_PER_SEC;
		if (wait_time >= 120)
			m_Pool->setThreadState(std::this_thread::get_id(), false);
	}
	if (m_Pool->getState() == stop_now || empty() || !m_Pool->getThreadState(std::this_thread::get_id()))
		return nullptr;
	Task* task = m_TaskQueue.top();
	m_TaskRunState[task] = true;
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
			if (tem != nullptr)
				delete tem;
			tem = nullptr;
		}
	}
	m_QueueNotEmpty.notify_all();
	m_QueueNotFull.notify_all();
}

void TaskQueue::notifyThreads()
{
	m_QueueNotEmpty.notify_all();
}

bool TaskQueue::cancelTask(Task* task)
{
	std::unique_lock<std::mutex> lock(m_Mtx);
	if (task == nullptr || dynamic_cast<CancelTask*> (task) == NULL || m_TaskRunState[task])
		return false;
	delete task;
	task = new CancelTask();
}