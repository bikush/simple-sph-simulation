
#include "DataLineSet.h"

using namespace std;

DataLineSet::DataLineSet()
{
	lineMap = map< string, DataLine >();
}

bool DataLineSet::readLine( istream& is )
{
	string lineName;
	is >> lineName;
	DataLine data = DataLine( is );
	if( lineMap.find( lineName ) == lineMap.end() )
	{
		lineMap[ lineName ] = data;
		return true;
	}else
	{
		return false;
	}
}

DataLine DataLineSet::getData( string name )
{
	DataLine data;
	map< string, DataLine >::iterator foundData = lineMap.find( name );
	if( foundData != lineMap.end() )
	{
		data = foundData->second;
	}
	return data;
}