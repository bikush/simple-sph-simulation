
#include "DataLineSet.h"

using namespace std;

DataLineSet::DataLineSet()
{
}

bool DataLineSet::readLine( istream& is )
{
	string lineName;
	is >> lineName;
	DataLine data( is );
	if( lineMap.find( lineName ) == lineMap.end() )
	{
		lineMap[ lineName ] = data;
		return true;
	}else
	{
		return false;
	}
}

const DataLine& DataLineSet::getData( string name ) const
{
	auto foundData = lineMap.find( name );
	return foundData != lineMap.end() ? foundData->second : DataLine::getDefault();
}