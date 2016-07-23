#pragma once
#ifndef _MARCHING_SQUARES_H
#define _MARCHING_SQUARES_H

#include "MarchingSquaresBase.h"

/*
* Simple Marching squares algorithm implementation. Node values are true/false. 
* Dynamic memory managment.Equivalent to Marching Smooth Squares with parameter
* maxValue set to 1.
*
* Functions for grid and square drawing, setting and flipping field elements,
* drawing circles and a rectangles to the field.
*
* Uses Marching Square Factory for fast triangle generating and glDrawArray for 
* fast drawing.
*
* Improvments: better memory management (without array copying), bitfield instead
* of bool array.
*/
class MarchingSquares : public MarchingSquaresBase
{
	
public:
	MarchingSquares( );
	MarchingSquares(int w, int h, glm::vec3 p = glm::vec3(0,0,0), glm::vec3 s = glm::vec3(1,1,1));
	~MarchingSquares();

	void set( int x, int y, float value = 1 );
	void dec( int x, int y, float value = 1 );
	void flip( int x, int y );
	void putRectangle( int x1, int y1, int x2, int y2, bool value = true );
	void clear( );

	void drawGrid( glm::vec3 colorFalse = glm::vec3(1,0,0), glm::vec3 colorTrue = glm::vec3(0,1,0) );	
		
private:
	bool* dataField;
	
	int generateTrianglesInField( int x, int y, int startIndex );

};


#endif