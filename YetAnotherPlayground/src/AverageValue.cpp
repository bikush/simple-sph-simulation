
#include "AverageValue.h"
#include <iostream>

void AverageValue::addValue( int aCount, double value )
{
	total += value;
	count += aCount;
	if( count > period )
	{
		count -= period;
		double difference = (total - total * count / (count + period));
		total -= difference;
		average = difference / period;
	//	std::cout <<"Total:" << total << " Average:" << average << std::endl;
	}
}

double AverageValue::currentAverage()
{
	return average;
}