
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
	
string DataLine::getStringData( string fallback, string joiner) const
{
	if( lineData.size() == 0 )
	{
		return fallback;
	}
	return join( lineData, joiner );
}

glm::vec2 DataLine::getVec2() const
{
	glm::vec2 out;
	if( lineData.size() >= 2 )
	{
		out[0] = readString<float>( lineData[0] );
		out[1] = readString<float>( lineData[1] );
	}
	return out;
}

glm::vec3 DataLine::getVec3() const
{
	glm::vec3 out;
	if( lineData.size() >= 3 )
	{
		out[0] = readString<float>( lineData[0] );
		out[1] = readString<float>( lineData[1] );
		out[2] = readString<float>( lineData[2] );
	}
	return out;
}
