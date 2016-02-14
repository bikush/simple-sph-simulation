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
	std::vector<std::string> lineData;
	
public:
	DataLine();
	DataLine( std::istream &is );

	DataLine& operator=( DataLine& in );
	
	std::string getStringData(std::string fallback = "", std::string joiner = " ");
	std::vector< float > getFloatVector();
	float getFloat( float fallback = 0.0f );
	int getInt( int fallback = 0 );
	std::vector< std::string > getStringVector();
	vec2f getVec2f();
	vec3f getVec3f();

	void fillFloatArray( float* in, int count );
};

#endif