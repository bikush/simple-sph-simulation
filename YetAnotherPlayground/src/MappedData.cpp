
#include "MappedData.h"

using namespace std;

void MappedData::loadFromStream( istream& is )
{		
	string groupName = "default";
	char next;
	while( !is.eof() )
	{
		is >> next;			
		if( next == '[' )
		{
			is >> groupName;
			groupName.erase( groupName.size()-1 );
		}
		else
		{
			is.putback( next );
		}
		dataMap[ groupName ].readLine( is );
	}
}

MappedData::MappedData()
{
}

MappedData::MappedData( const char* filePath )
{
	ifstream mapFile(filePath);
	if( mapFile.is_open() )
	{
		loadFromStream( mapFile );
	}
}

MappedData::MappedData( istream& is )
{
	loadFromStream( is );
}
	
const DataLine& MappedData::getData( string groupName, string dataName ) const
{		
	auto group = dataMap.find( groupName );
	if( group != dataMap.end() )
	{
		return group->second.getData( dataName );
	}
	return DataLine::getDefault();
}
