#include "stdafx.h"
#include "task.h"

int Task::m_CurrentId = 1;

Task::Task()
	: m_Id(m_CurrentId++), m_Priority(0)
{
}


Task::~Task()
{
}

void Task::setPriority(int priority)
{
    m_Priority = priority;
}

int Task::getPriority()
{
    return m_Priority;
}

int Task::getId()
{
    return m_Id;
}