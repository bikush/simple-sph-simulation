
#include "DataLine.h"
#include <algorithm>

using namespace std;

DataLine::DataLine()
{
}

DataLine::DataLine( istream &is )
{		
	string data;
	while( is.peek() != '\n' && !is.eof() )
	{
		is >> data;
		lineData.push_back( data );
	}
}

DataLine& DataLine::operator=( DataLine& in )
{		
	lineData = in.lineData;
	return *this;
}
	
string DataLine::getStringData( string fallback, string joiner)
{
	if( lineData.size() == 0 )
	{
		return fallback;
	}
	return join( lineData, joiner );
}

vec2f DataLine::getVec2f()
{
	vec2f out;
	if( lineData.size() >= 2 )
	{
		out[0] = readString<float>( lineData[0] );
		out[1] = readString<float>( lineData[1] );
	}
	return out;
}

vec3f DataLine::getVec3f()
{
	vec3f out;
	if( lineData.size() >= 3 )
	{
		out[0] = readString<float>( lineData[0] );
		out[1] = readString<float>( lineData[1] );
		out[2] = readString<float>( lineData[2] );
	}
	return out;
}
