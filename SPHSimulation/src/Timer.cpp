
#include "Timer.h"

Timer *Timer::instance = 0;

// PUBLIC METHODS

Timer* Timer::getInstance()
{
	if( !instance )
		instance = new Timer();
	return instance;
}

void Timer::calcTimeFlow()
{
	getInstance()->_calcTimeFlow();
}

bool Timer::isPaused()
{
	return getInstance()->paused;
}

double Timer::pauseToggle()
{
	return getInstance()->_pauseToggle();
}

int Timer::getFPS()
{
	return getInstance()->lastFPS;
}

double Timer::getDT()
{
	return getInstance()->dT;
}

long Timer::getSeconds()
{
	return getInstance()->totalSeconds;
}

double Timer::getTime()
{
	return getInstance()->totalTime;
}

// Creates and starts a timer. Returns timer ID.
int Timer::createTimer()
{
	Timer* t = getInstance();
	int index = t->nextTimerIndex;
	t->nextTimerIndex++;
	t->timerMap[ index ] = clock();
	return index;
}

// Stops timerID timer and returns time passed.
double Timer::stopTimerD( int timerID )
{
	return double(Timer::stopTimerT( timerID )/CLOCKS_PER_SEC);
}

time_t Timer::stopTimerT( int timerID )
{
	Timer* t = getInstance();
	std::map< int, time_t >::iterator time = t->timerMap.find( timerID );
	time_t out = 0;
	if( time != t->timerMap.end() )
	{
		out = clock() - time->second;
		t->timerMap.erase( time );
	}
	return out;
}

// PRIVATE METHODS

Timer::Timer()
{
	time = 0.0025f;		
	dT = 0.05f;	
	totalTime = 0;
	start = clock();

	paused = false;
	timePaused = 0.0f;
	
	FPS = 0;
	lastFPS = 0;
	
	secondCounter = 1.0f;
	totalSeconds = 0;

	timerMap = std::map<int, time_t>();
	nextTimerIndex = 0;
}

void Timer::_calcTimeFlow()
{
	time_t now = clock();
	double difference = double (now - start)/CLOCKS_PER_SEC;
	if(difference > time){
		secondCounter -= difference;
		if(secondCounter < 0)
		{
			secondCounter = 1.0f;
			totalSeconds++;
			lastFPS = FPS;
			FPS = 0;
		}else
		{
			FPS++;
		}
		start = now;
		dT = difference;	
		if( paused )
		{
			timePaused += difference;
			dT = 0.0f;
		}
		totalTime += difference;
	}			
}


double Timer::_pauseToggle()
{
	paused = !paused;
	if( paused )
	{
		timePaused = 0.0f;
	}
	return timePaused;
}



