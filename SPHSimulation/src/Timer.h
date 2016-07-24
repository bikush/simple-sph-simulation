#pragma once
#ifndef _TIMER_H
#define _TIMER_H

#include <chrono>

class FPSTimer {
private:
	typedef std::chrono::high_resolution_clock::time_point time_point;

	time_point start;
	long long ticks;
	long long clockInterval;	// in microseconds
	int lastFPS;

public:
	FPSTimer(double interval);
	void tick();
	int fps();
};

class Timer{
private:
	typedef std::chrono::high_resolution_clock::time_point time_point;
	time_point start;
public:
	Timer();
	double elapsed();

	template<class duration>
	long long ticks() {
		time_point now = std::chrono::high_resolution_clock::now();
		return std::chrono::duration_cast<duration>(now - start).count();
	}
};


#endif