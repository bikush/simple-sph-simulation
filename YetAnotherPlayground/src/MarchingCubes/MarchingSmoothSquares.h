#pragma once
#ifndef _MARCHING_SMOOTH_SQUARES_H
#define _MARCHING_SMOOTH_SQUARES_H

#include "MarchingSquaresBase.h"

class MarchingSmoothSquares : public MarchingSquaresBase
{
public:
	MarchingSmoothSquares( );
	MarchingSmoothSquares(int w, int h, char max, vec3f p = vec3f(0,0,0), vec3f s = vec3f(1,1,1));
	MarchingSmoothSquares( const char* file );
	~MarchingSmoothSquares();

	void set( int x, int y, float value = 1 );
	void inc( int x, int y, char value = 1 );
	void dec( int x, int y, float value = 1 );
	void clear( );
	
	void drawGrid( vec3f colorFalse = vec3f(1,0,0), vec3f colorTrue = vec3f(0,1,0) );	
	
private:
	char* dataField;
	char maxValue;

	int generateTrianglesInField( int x, int y, int startIndex );
};


#endif