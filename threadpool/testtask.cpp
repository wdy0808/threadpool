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
	printf("%d running!\n", m_TestNum);
	Sleep(10000);
}