#include "stdafx.h"
#include "testtask.h"
#include <Windows.h>

testtask::testtask(int num)
	: m_TestNum(num)
{
}


testtask::~testtask()
{
}

void testtask::run()
{
	printf("%d running! %d\n", m_TestNum, std::this_thread::get_id());
	Sleep(5000);
	printf("%d done!\n", m_TestNum);
}