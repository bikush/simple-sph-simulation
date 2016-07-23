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
	AverageValue( int timePeriod = 1 ) : 
		period(timePeriod), total(0.0), average(0.0), count(0) 
	{};

	void addValue( int aCount, double value )
	{
		total += value;
		count += aCount;
		if (count > period)
		{
			count -= period;

			// value of Total might be calculated over a count longer than the period
			// for the average we can use the whole value, but the total will be reduced
			// only by expected period value
			average = total / (count + period);
			total -= average * period;
		}
	}

	double getAverage()
	{
		return average;
	}
};


#endif