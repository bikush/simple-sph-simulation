#pragma once
#ifndef INTERVAL_AVERAGE_TIMER_H
#define INTERVAL_AVERAGE_TIMER_H

#include "Timer.h"
#include <time.h>

class IntervalAverageTimer
{
	time_t timeAccumulator;
	double tickCounter;
	long lastSeconds;
	double lastAverage;
	int timerID;
	long intervalLength;

public:
	IntervalAverageTimer( long interval = 1 )
	{
		lastSeconds = Timer::getSeconds();
		timeAccumulator = 0;
		tickCounter = 0;
		lastAverage = 0;
		intervalLength = interval;
		timerID = -1;
	}

	void pause()
	{
		if( timerID == -1 ) return;
		timeAccumulator += Timer::stopTimerT( timerID );
		tickCounter++;
		timerID = -1;
	}

	void resume()
	{
		timerID = Timer::createTimer();
	}

	double getAverage()
	{
		long newSeconds = Timer::getSeconds();
		long passedSeconds = newSeconds - lastSeconds;
		if( passedSeconds >= intervalLength )
		{
			lastAverage = timeAccumulator /( CLOCKS_PER_SEC * (tickCounter==0?1:tickCounter) );
			timeAccumulator = 0;
			tickCounter = 0;
			lastSeconds = newSeconds;
		}
		return lastAverage;
	}
};

#endif