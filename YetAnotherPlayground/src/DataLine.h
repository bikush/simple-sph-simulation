#pragma once
#ifndef _DATA_LINE_H
#define _DATA_LINE_H

#include <vector>
#include <string>

#include "Utility.h"
#include "Vector2D.h"
#include "Vector3D.h"
#include <glm\vec2.hpp>
#include <glm\vec3.hpp>

class DataLine
{
private:
	std::vector<std::string> lineData;
	
public:
	DataLine();
	DataLine( std::istream &is );

	DataLine& operator=( DataLine& in );
	
	std::string getStringData(std::string fallback = "", std::string joiner = " ");
	vec2f getVec2f();
	vec3f getVec3f();

	template<class T>
	T get( T fallback = T() )
	{
		return lineData.size() > 0 ? readString<T>(lineData[0]) : fallback;
	}

	template<class T>
	std::vector<T> getVector()
	{
		std::vector<T> data;
		for (auto& line : lineData)
		{
			data.push_back(readString<T>(line));
		}
		return data;
	}

	template<>
	std::vector<std::string> getVector()
	{
		return lineData;
	}

	template<class T>
	void fillArray( T* in, int count )
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

};

#endif