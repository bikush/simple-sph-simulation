#pragma once
#ifndef MAPPED_DATA_H
#define MAPPED_DATA_H

#include <string>
#include <map>

#include "DataLine.h"
#include "DataLineSet.h"

class MappedData
{
private:
	std::map< std::string, DataLineSet > dataMap;

	void loadFromStream( std::istream& is );

public:
	MappedData();
	MappedData( const char* filePath );
	MappedData( std::istream& is );

	const DataLine& getData( std::string groupName, std::string dataName ) const;
};

#endif