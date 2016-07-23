#pragma once
#ifndef _DATA_LINE_H
#define _DATA_LINE_H

#include <vector>
#include <string>

#include "Utility.h"
#include "GlmVec.h"

class DataLine
{
private:
	std::vector<std::string> lineData;
	
public:
	DataLine();
	DataLine( std::istream &is );

	DataLine& operator=( DataLine& in );
	
	std::string getStringData(std::string fallback = "", std::string joiner = " ") const;
	glm::vec2 getVec2() const;
	glm::vec3 getVec3() const;

	template<class T>
	T get( T fallback = T() ) const
	{
		return lineData.size() > 0 ? readString<T>(lineData[0]) : fallback;
	}

	template<class T>
	std::vector<T> getVector() const
	{
		std::vector<T> data;
		for (auto& line : lineData)
		{
			data.push_back(readString<T>(line));
		}
		return data;
	}

	template<>
	std::vector<std::string> getVector() const
	{
		return lineData;
	}

	template<class T>
	void fillArray( T* in, int count ) const
	{
		if (in != nullptr)
		{
			count = max(count, (int)lineData.size());
			for (int i = 0; i<count; i++)
			{
				in[i] = readString<T>(lineData[i]);
			}
		}		
	}

	static const DataLine& getDefault()
	{
		static DataLine empty;
		return empty;
	}

};

#endif