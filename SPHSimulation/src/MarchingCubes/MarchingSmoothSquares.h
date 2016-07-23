#pragma once
#ifndef _MARCHING_SMOOTH_SQUARES_H
#define _MARCHING_SMOOTH_SQUARES_H

#include "MarchingSquaresBase.h"

class MarchingSmoothSquares : public MarchingSquaresBase
{
public:
	MarchingSmoothSquares( );
	MarchingSmoothSquares(int w, int h, char max, glm::vec3 p = glm::vec3(0,0,0), glm::vec3 s = glm::vec3(1,1,1));
	MarchingSmoothSquares( const char* file );
	~MarchingSmoothSquares();

	void set( int x, int y, float value = 1 );
	void inc( int x, int y, char value = 1 );
	void dec( int x, int y, float value = 1 );
	void clear( );
	
	void drawGrid( glm::vec3 colorFalse = glm::vec3(1,0,0), glm::vec3 colorTrue = glm::vec3(0,1,0) );	
	
private:
	char* dataField;
	char maxValue;

	int generateTrianglesInField( int x, int y, int startIndex );
};


#endif