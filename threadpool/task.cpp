#include "stdafx.h"
#include "task.h"


Task::Task()
	: m_Priority(10)
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

void CancelTask::run()
{
}