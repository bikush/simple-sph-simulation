#pragma once
#ifndef MARCHING_SQUARES_BASE_H
#define MARCHING_SQUARES_BASE_H

#include "GlmVec.h"
#include "MappedData.h"

/**
 * Base class for marching squares algorithm. Derived classes define data type and
 * data managing methods.
 *
 * Main commonality of marching squares algorithm is datafield traversal and triangle
 * buffer drawing.
 *
 * Improvments: Define set and dec methods better. Improve memory management for
 * triangle buffer. Faster drawing? Could textures be used?
 *
 * Author: BM
 **************/
class MarchingSquaresBase
{
public:
	MarchingSquaresBase(void);	
	// Param: width and height of the data field, position and scale for drawing.
	MarchingSquaresBase(int w, int h, glm::vec3 p = glm::vec3(0,0,0), glm::vec3 s = glm::vec3(1,1,1));	
	MarchingSquaresBase( MappedData& paramFile );
	virtual ~MarchingSquaresBase(void);
	
	// Param: (x,y) circle center position in the data field and radius of the circle.
	void putCircle( int x, int y, int r );	
	// Param: (x,y) circle center position in the data field and radius of the circle.
	// More flexible than integer version.
	void putCircle( float x, float y, float r, bool dummy);
	// Param: (x,y) data field position and radius of the circle.
	void removeCircle( int x, int y, int r );

	// Param: (x,y) data field position and value <0..1> to set.
	virtual void set( int x, int y, float value )=0;	
	// Param: (x,y) data field position and value <0..1> to decrease.
	virtual void dec( int x, int y, float value )=0;
	virtual void clear( )=0;

	// Param: colors for 0 valued fields and non-0 valued fields.
	virtual void drawGrid( glm::vec3 colorFalse = glm::vec3(1,0,0), glm::vec3 colorTrue = glm::vec3(0,1,0) )=0;				
	// Param: color for the squares.
	void drawSquares( glm::vec3 color );
		
	int getWidth( );
	int getHeight( );

protected:
	// Used for triangle field management when initializing and resizing.
	static const int TRIANGLE_COUNT_INCREASE = 256;

	int dataWidth;
	int dataHeight;
	// Flag to prevent unneccesary triangle generating.
	bool dataChanged;

	// Position in 3D space.
	glm::vec3 position;
	// Scale in 3D space.
	glm::vec3 scale;

	// Triangle mesh used to draw the squares. Multiple occurence of vertices. Non-optimal resizing.
	glm::vec2* triangles;
	int trianglesCount;
	int trianglesSize;

	// Every derived class implements its own way of triangle generating (using MarchingSquaresFactory).
	virtual int generateTrianglesInField( int x, int y, int startIndex )=0;
		
	// Used to refresh the triangle field when data field changes.
	void generateTriangles();

private:
	// Used by put/removeCircle with different setter function.
	void _putCircleUsingSetter( int x, int y, int r, void (MarchingSquaresBase::*setter)(int,int,float) );
};

#endif
