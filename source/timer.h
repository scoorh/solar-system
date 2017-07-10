#ifndef __TIMER_H__
#define __TIMER_H__


#include "common.h"


class Timer {
	double spc;
	double dt;

	__int64 pt;

public:
	Timer();

	void reset(); 
	void tick(); 

	float delta_time() const; 
};


#endif