#pragma once
#ifndef INTERVAL_AVERAGE_TIMER_H
#define INTERVAL_AVERAGE_TIMER_H

#include <chrono>

/*
 * Calculates durations over a certain interval given in seconds.
 * Durations are accumulated between calls to pause/resume.
 * Average is given depending on the number of pause/resume calls during the interval.
 */
class IntervalAverageTimer
{
	typedef std::chrono::high_resolution_clock::time_point time_point;

	time_point start;
	time_point intervalStart;
	long long accumulator;	// microseconds
	bool paused;

	double tickCounter;
	double lastAverage;
	long long intervalLength;

public:
	IntervalAverageTimer( long interval = 1 ) :
		start( std::chrono::high_resolution_clock::now() ), intervalStart(start),
		paused(false), tickCounter(0), lastAverage(0), intervalLength(interval)
	{
	}

	void pause()
	{
		if (paused) {
			return;
		}
		paused = true;
		time_point now = std::chrono::high_resolution_clock::now();

		accumulator += std::chrono::duration_cast<std::chrono::microseconds>(now-start).count();
		tickCounter++;
	}

	void resume()
	{
		if (!paused) {
			return;
		}
		paused = false;
		start = std::chrono::high_resolution_clock::now();
	}

	double getAverage()
	{
		time_point now = std::chrono::high_resolution_clock::now();
		auto passedSeconds = std::chrono::duration_cast<std::chrono::seconds>(now - intervalStart).count();
		if( passedSeconds >= intervalLength )
		{
			bool wasPaused = paused;
			pause();

			lastAverage = accumulator /( 1e6 * (tickCounter==0?1:tickCounter) );
			accumulator = 0;
			tickCounter = 0;
			intervalStart += std::chrono::seconds(intervalLength);

			// Mid interval average calculations need to pause and resume to keep average correct
			if (!wasPaused) {
				resume();
			}
		}
		return lastAverage;
	}
};

#endif