#include "stdafx.h"
#include "timer.h"
#include "poolcondition.h"

Timer::Timer(PoolCondition* pool)
	: m_Expire(true), m_Pool(pool)
{
}


Timer::~Timer()
{
}

void Timer::startTimer(int second, std::function<void()> task)
{
    m_Expire = false;
    std::thread([this, second, task](){
        std::this_thread::sleep_for(std::chrono::seconds(second));
        if (!m_Expire && m_TimerNum == 1)
            task();
        m_TimerNum--;
	}).detach();
}

bool Timer::getTimerExpireState()
{
    return m_Expire;
}
