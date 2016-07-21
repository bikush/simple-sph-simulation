#pragma once
#ifndef MARCHING_SQUARES_FACTORY_H
#define MARCHING_SQUARES_FACTORY_H

#include "GlmVec.h"
#include <vector>

// Static class that outputs tiles for the Marching Squares algorithm.
class MarchingSquaresFactory
{
	// Vector of vertices that are used to output the triangles.
	// (2)(3)(4)
	// (1)   (5)  - numbering of output vertices representing square vertices and edges
	// (0)(7)(6)  - 0,2,4,6 are square vertices; 1,3,5,7 are square edges
	static glm::vec2 vertices[8];
	// Triangle edges for each of the 16 possible tile cases.
	static int squareCases[16][13];
	// Offset vectors used for interpolated squares.
	static glm::vec2 interpolateOffset[9];

public:
	// Maximum number of vertices generated for a tile case.
	static const int MAX_VERTICES = 12;

	// Fills the given triangle buffer with triangle vertices of the marching squares tile that
	// matches given indices. Offsets square edge vertices based on the maximum value neighbouring vertices can hold.
	// Input:
	//		indices - values in vertices of a square, ordering is: (x,y),(x,y+1),(x+1,y+1),(x+1,y)
	//		triangles - output buffer, must have enough space for at least MAX_VERTICES glm::vec2 elements
	//		start - starting index in triangle buffer
	//		offset - added to the generated triangles
	//		maxValue - upper limit of value in indices
	// Returns: number of generated vertices
	static int getInterpolatedSquare( char indices[4], glm::vec2* triangles, int start, glm::vec2 offset, char maxValue = 1 );
		
	// Fills the given triangle buffer with triangle vertices of the marching squares tile that
	// matches given indices.
	// Input:
	//		square values: index - (x,y), up - (x,y+1), diag - (x+1,y+1), right - (x+1,y)
	//		triangles - output buffer 
	//		offset - added to the generated triangles
	static void getSquare( bool index, bool up, bool diag, bool right, std::vector<glm::vec2>& triangles, glm::vec2 offset );

	// Fills the given triangle buffer with triangle vertices of the marching squares tile that
	// matches given indices.
	// Input:
	//		square values: index - (x,y), up - (x,y+1), diag - (x+1,y+1), right - (x+1,y)
	//		triangles - output buffer, must have enough space for at least MAX_VERTICES glm::vec2 elements
	//		start - starting index in triangle buffer
	//		offset - added to the generated triangles
	// Returns: number of generated vertices
	static int getSquare( bool index, bool up, bool diag, bool right, glm::vec2* triangles, int start, glm::vec2 offset );

	// Fills the given triangle buffer with triangle vertices of the marching squares tile that
	// matches given indices.
	// Input:
	//		squareIndex - marching square tile case
	//		triangles - output buffer 
	//		offset - added to the generated triangles
	static void getSquare( int squareIndex, std::vector<glm::vec2>& triangles, glm::vec2 offset  );

	// Fills the given triangle buffer with triangle vertices of the marching squares tile that
	// matches given indices.
	// Input:
	//		squareIndex - marching square tile case
	//		triangles - output buffer, must have enough space for at least MAX_VERTICES glm::vec2 elements
	//		start - starting index in triangle buffer
	//		offset - added to the generated triangles
	// Returns: number of generated vertices
	static int getSquare( int squareIndex, glm::vec2* triangles, int start, glm::vec2 offset );
};


#endif