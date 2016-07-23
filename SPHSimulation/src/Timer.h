#pragma once
#ifndef _TIMER_H
#define _TIMER_H

#include <time.h>
#include <map>

class Timer{
public:
	static Timer* getInstance();
	static void calcTimeFlow();
	static bool isPaused();
	static double pauseToggle();
	static long getSeconds();
	static int getFPS();
	static double getDT();
	static double getTime();

	// Creates and starts a timer. Returns timer ID.
	static int createTimer();
	// Stops timerID timer and returns time passed.
	static double stopTimerD( int timerID );
	static time_t stopTimerT( int timerID );

private:
	Timer();
	void _calcTimeFlow();
	double _pauseToggle();
	
	static Timer* instance;

	double time;
	time_t start;
	double dT;
	double totalTime;

	bool paused;
	double timePaused;

	double secondCounter;
	long totalSeconds;

	int FPS;
	int lastFPS;

	std::map< int, time_t > timerMap;
	int nextTimerIndex;	
};


#endif