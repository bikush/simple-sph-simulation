
#include "MarchingSquaresFactory.h"

void MarchingSquaresFactory::getSquare( bool index, bool up, bool diag, bool right, std::vector<glm::vec2>& triangles, glm::vec2 offset )
{
	getSquare( (index?8:0)+(up?4:0)+(diag?2:0)+(right?1:0), triangles, offset );
}

int MarchingSquaresFactory::getSquare( bool index, bool up, bool diag, bool right, glm::vec2* triangles, int start, glm::vec2 offset )
{
	return getSquare( (index?8:0)+(up?4:0)+(diag?2:0)+(right?1:0), triangles, start, offset );
}
	
void MarchingSquaresFactory::getSquare( int squareIndex, std::vector<glm::vec2>& triangles, glm::vec2 offset  )
{
	int count = 0;

	while( squareCases[ squareIndex ][count] != -1 )
	{
		triangles.push_back( vertices[ squareCases[squareIndex][count] ] + offset );
		count++;
	}
}

int MarchingSquaresFactory::getSquare( int squareIndex, glm::vec2* triangles, int start, glm::vec2 offset )
{		
	int count = 0;

	while( squareCases[ squareIndex ][count] != -1 )
	{
		triangles[start+count] = vertices[ squareCases[squareIndex][count] ] + offset;
		count++;
	}

	return count;
}

int MarchingSquaresFactory::getInterpolatedSquare( char indices[4], glm::vec2* triangles, int start, glm::vec2 offset, char maxValue )
{
	int squareIndex = 0;
	for(int i=8, j=0; j<4; i/=2, j++) if( indices[j] != 0 ) squareIndex += i;

	int count = 0;	// return value, counts number of generated vertices
	int chosenVertice = squareCases[ squareIndex ][count];	// vertice that is copied into triangle buffer
	int interpOff;	// index of the matching offset from interpolateOffset array
	float interpFac;	// factor for the offset, [0..1]
	while( chosenVertice != -1 )
	{
		if( (chosenVertice & 0x1 ) == 1 )	// edge is chosen
		{
			int next = ((chosenVertice+1)/2) % 4;	// after an edge is a vertice with index 'next'
			int prev = ((chosenVertice-1)/2);		// before an edge is a vertice with index 'prev'
			if( indices[ next ] < indices[ prev ] )	// compare values in next and previous vertices to choose offset
			{
				interpOff = chosenVertice-1;				
				interpFac = 1-indices[prev]/(float)maxValue;
			}else
			{
				interpOff = chosenVertice;
				interpFac = 1-indices[next]/(float)maxValue;
			}
		}else								// square vertice is chosen
		{
			interpOff = 8;
			interpFac = 0;
		}

		triangles[start+count] = 
			vertices[ squareCases[squareIndex][count] ] + 
			interpolateOffset[interpOff]*interpFac + 
			offset;

		count++;
		chosenVertice = squareCases[ squareIndex ][count];
	}

	return count;
}

glm::vec2  MarchingSquaresFactory::interpolateOffset[] = {
	glm::vec2( 0, -0.5 ),
	glm::vec2( 0,  0.5 ),
	glm::vec2( -0.5, 0 ),
	glm::vec2( 0.5, 0 ),
	glm::vec2( 0,  0.5 ),
	glm::vec2( 0, -0.5 ),
	glm::vec2( 0.5, 0 ),
	glm::vec2( -0.5, 0 ),	
	glm::vec2( 0,0 )
};

glm::vec2 MarchingSquaresFactory::vertices[] = {
		glm::vec2( 0 , 0 ),
		glm::vec2( 0, 0.5 ),
		glm::vec2( 0, 1 ),
		glm::vec2( 0.5, 1 ),
		glm::vec2( 1, 1 ),
		glm::vec2( 1, 0.5 ),
		glm::vec2( 1, 0 ),
		glm::vec2( 0.5, 0 )
};

int MarchingSquaresFactory::squareCases[16][13] = {
	{-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},	//  0 none
	{ 5, 6, 7,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},	//  1 only right
	{ 3, 4, 5,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},	//  2 only diag
	{ 4, 6, 7, 7, 3, 4,-1,-1,-1,-1,-1,-1,-1},	//  3 rectangle diag+right
	{ 1, 2, 3,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},	//  4 only up
	{ 6, 7, 1, 6, 1, 2, 6, 2, 3, 6, 3, 5,-1},	//  5 diagonal up+right
	{ 1, 2, 4, 4, 5, 1,-1,-1,-1,-1,-1,-1,-1},	//  6 rectangle up+diag
	{ 4, 6, 7, 4, 7, 1, 4, 1, 2,-1,-1,-1,-1},	//  7 three
	{ 0, 1, 7,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},	//  8 only index
	{ 5, 6, 0, 0, 1, 5,-1,-1,-1,-1,-1,-1,-1},	//  9 rectangle index+right
	{ 0, 1, 3, 0, 3, 4, 0, 4, 5, 0, 5, 7,-1},	// 10 diagonal index+diag
	{ 6, 0, 1, 6, 1, 3, 6, 3, 4,-1,-1,-1,-1},	// 11 three
	{ 0, 2, 3, 3, 7, 0,-1,-1,-1,-1,-1,-1,-1},	// 12 rectangle index+up
	{ 0, 2, 3, 0, 3, 5, 0, 5, 6,-1,-1,-1,-1},	// 13 three
	{ 2, 4, 5, 2, 5, 7, 2, 7, 0,-1,-1,-1,-1},	// 14 three
	{ 0, 2, 6, 6, 2, 4,-1,-1,-1,-1,-1,-1,-1}	// 15 all
};				
	
