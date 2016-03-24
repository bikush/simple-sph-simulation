#pragma once
#ifndef _DATA_LINE_SET_H
#define _DATA_LINE_SET_H

#include <string>
#include <map>

#include "DataLine.h"

class DataLineSet
{
private:
	std::map< std::string, DataLine > lineMap;

public:
	DataLineSet();

	bool readLine(std::istream& is );
	const DataLine& getData(std::string name ) const;
};

#endif