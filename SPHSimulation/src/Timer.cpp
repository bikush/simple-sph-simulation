
#include "Timer.h"

using namespace std::chrono;

const double MSEC_PER_SEC = 1e6;

FPSTimer::FPSTimer(double interval) :
	start(high_resolution_clock::now()),
	clockInterval( long(interval * MSEC_PER_SEC) ), ticks(0), lastFPS(0)
{

}

void FPSTimer::tick() 
{
	time_point now = high_resolution_clock::now();
	auto passedTicks = duration_cast<microseconds>(now - start).count();
	if (passedTicks > clockInterval) {
		start += microseconds(clockInterval);
		lastFPS = int(ticks * MSEC_PER_SEC / clockInterval);
		ticks = 0;
	}
	ticks++;
}

int FPSTimer::fps() 
{
	return lastFPS;
}

Timer::Timer() : 
	start(high_resolution_clock::now())
{
}

double Timer::elapsed()
{
	auto passedTicks = ticks<microseconds>();
	return passedTicks / MSEC_PER_SEC;
}