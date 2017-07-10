#include "timer.h"



Timer::Timer()
: spc(0.0), dt(-1.0),pt(0)
{
	__int64 cps;
	QueryPerformanceFrequency((LARGE_INTEGER*)&cps);
	spc = 1.0 / (double)cps;
}


float Timer::delta_time()const
{
	return (float)dt;
}


void Timer::reset()
{
	__int64 currTime;
	QueryPerformanceCounter((LARGE_INTEGER*)&currTime);
	pt = currTime;
}



void Timer::tick()
{
	__int64 ct;
	QueryPerformanceCounter((LARGE_INTEGER*)&ct);

	dt = (ct - pt)*spc;
	pt = ct;

	if (dt < 0.0) dt = 0.0;
}

