#include "stdafx.h"
#include "poolcondition.h"
#include "taskqueue.h"
#include "threadsworker.h"


PoolCondition::PoolCondition(ThreadIncreaseWay way, int start, int max, int maxtask, int add)
	: m_Way(way), m_StartNum(start), m_MaxThreadNum(max),
	m_IncreaseNum(add), m_Task(new TaskQueue(maxtask, this)), 
	m_Thread(new ThreadsWorker(this)), m_WorkedNum(0), m_State(normal)
{
	activiteMoreThread(true);
}


PoolCondition::~PoolCondition()
{
	if (m_Task != nullptr)
	{
		delete m_Task;
		m_Task = nullptr;
	}
	if (m_Thread != nullptr)
	{
		delete m_Thread;
		m_Thread = nullptr;
	}
}

void PoolCondition::activiteMoreThread(bool initialize)
{
	if (initialize)
		m_WorkedNum += m_Thread->addThreads(m_StartNum);
	else
	{
		if (m_WorkedNum == m_MaxThreadNum)
			return;
		m_WorkedNum += m_Thread->addThreads(m_IncreaseNum);
		if (m_Way == increase_index)
			m_IncreaseNum *= 2;
	}
}

int PoolCondition::getWorkedThreadsNum()
{
	return m_WorkedNum;
}

int PoolCondition::getMaxThreadNum()
{
	return m_MaxThreadNum;
}

void PoolCondition::setIncreaseMode(ThreadIncreaseWay way, int num)
{
	m_Way = way;
	m_IncreaseNum = num;
} 

Task* PoolCondition::getTask()
{
	return m_Task->getTask();
}

bool PoolCondition::addTask(Task* task, bool ifblock)
{
	if (m_State == ThreadPoolState::stop || m_State == stop_now || task == nullptr)
		return false;
	if (ifblock)
		m_Task->addTask_Block(task);
	else
		return m_Task->addTask(task);
	return true;
}

void PoolCondition::release()
{
	m_WorkedNum--;
	if (m_Way == increase_index)
		m_IncreaseNum = 1;
}

ThreadPoolState PoolCondition::getState()
{
	return m_State;
}

void PoolCondition::stop()
{
	if (m_State == normal)
	{
		m_State = ThreadPoolState::stop;
		m_Task->stopQueueWork();
	}
}

void PoolCondition::shutdown()
{
	if (m_State == normal)
	{
		m_State = ThreadPoolState::stop_now;
		m_Task->stopQueueWork();
	}
}

bool PoolCondition::cancelTask(Task* task)
{
	return m_Task->cancelTask(task);
}