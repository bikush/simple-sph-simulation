
#include "DataLine.h"

using namespace std;

DataLine::DataLine()
{
	lineData = vector<string>();
}

DataLine::DataLine( istream &is )
{		
	string data;
	bool moreData = true;
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

vector< float > DataLine::getFloatVector()
{
	vector<float> data = vector<float>();
	for(unsigned int i=0; i<lineData.size(); i++ )
	{
		data.push_back( readString<float>( lineData[i] ) );
	}
	return data;
}

float DataLine::getFloat( float fallback )
{
	if( lineData.size() > 0 ){
		return readString<float>( lineData[0] );
	}else{
		return fallback;
	}
}

int DataLine::getInt( int fallback )
{
	if( lineData.size() > 0 ){
		return readString<int>( lineData[0] );
	}else{
		return fallback;
	}
}

vector< string > DataLine::getStringVector()
{
	return lineData;
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

void DataLine::fillFloatArray( float* in, int count )
{
	if( count > lineData.size())
	{
		count = lineData.size();
	}
	for(unsigned int i=0; i<count; i++)
	{
		in[i] = readString<float>( lineData[i] );
	}
}