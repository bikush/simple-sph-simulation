#pragma once
#ifndef _H_AVERAGE_VALUE
#define _H_AVERAGE_VALUE

class AverageValue
{
private:
	double total;
	double average;
	int count;
	int period;

public:
	AverageValue( int timePeriod = 1 ) : period(timePeriod), total(0.0), average(0.0), count(0) {};
	void addValue( int count, double value );

	double currentAverage();
};


#endif