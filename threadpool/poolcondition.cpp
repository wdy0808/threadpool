#include "stdafx.h"
#include "poolcondition.h"
#include "taskqueue.h"
#include "threadsworker.h"
#include <cmath>


PoolCondition::PoolCondition(ThreadIncreaseWay way, int start, int max, int maxtask, int add, bool d, double minus)
	: m_Way(way), m_StartNum(start), m_MaxThreadNum(max),
	m_IncreaseNum(add), m_DecreaseRate(minus), m_Task(new TaskQueue(maxtask, this)), 
	m_Thread(new ThreadsWorker(this)), m_WorkedNum(0), m_State(normal), m_Decrease(d)
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
	setState(pausing);
	if (initialize)
		m_WorkedNum += m_Thread->addThreads(m_StartNum);
	else
	{
		if (m_Way == once_all || activeAll())
			return;
		m_WorkedNum += m_Thread->addThreads(m_IncreaseNum);
		if (m_Way == increase_index)
			m_IncreaseNum *= 2;
	}
	setState(normal);
}

void PoolCondition::releaseThread()
{
	setState(pausing);
	if (m_Decrease)
	{
		int num = m_WorkedNum - m_WorkedNum*m_DecreaseRate;
		for (int i=0; i<m_MaxThreadNum && num>0; i++)
		{
			if (m_ThreadCondition[m_Thread->getThreadId(i)])
			{
				m_ThreadCondition[m_Thread->getThreadId(i)] = false;
				num--;
			}
		}
		m_Task->notifyThreads();
		m_WorkedNum = m_WorkedNum*m_DecreaseRate;
		m_IncreaseNum = pow(m_MaxThreadNum, 0.25);
	}
	setState(normal);
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

void PoolCondition::setDecreaseMode(bool mode)
{
	m_Decrease = mode;
}

Task* PoolCondition::getTask()
{
	return m_Task->getTask();
}

bool PoolCondition::addTask(Task* task, bool ifblock)
{
	if (m_State == ThreadPoolState::stop || m_State == stop_now)
		return false;
	if (ifblock)
		m_Task->addTask_Block(task);
	else
		return m_Task->addTask(task);
	return true;
}

void PoolCondition::setThreadState(std::thread::id id, bool state)
{
	m_ThreadCondition[id] = state;
}

bool PoolCondition::getThreadState(std::thread::id id)
{
	return m_ThreadCondition[id];
}

ThreadPoolState PoolCondition::getState()
{
	return m_State;
}

void PoolCondition::setState(ThreadPoolState state)
{
	m_State = state;
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
		for (int i=0; i<m_MaxThreadNum; i++)
		{
			m_ThreadCondition[m_Thread->getThreadId(i)] = false;
		}
		m_Task->stopQueueWork();
	}
}

bool PoolCondition::activeAll()
{
	return m_WorkedNum == m_MaxThreadNum;
}