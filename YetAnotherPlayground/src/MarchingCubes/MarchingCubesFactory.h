#pragma once
#ifndef _MARCHING_CUBES_FACTORY_H
#define _MARCHING_CUBES_FACTORY_H

#include "Vector3D.h"
#include <GL\glew.h>

class MarchingCubesFactory
{	
	static GLuint texEdgeTable;
	static GLuint texTriTable;

	static vec3f cubeVertices[8];	// cube vertices
	static vec3f cubeEdges[12];	// cube edges
	static vec3f cubeEdgesOffset[12];	// cube edges offset direction
	
		
public:
	static const int MAX_VERTICES = 15;
	static int triTable[256][16];
	static int edgeTable[256];
		
	static int getCube( int cubeIndex, vec3f* triangles, vec3f* normals, int start, vec3f offset );
	static int getInterpolatedCube( char verticeValues[8], char maxValue, vec3f* triangles, vec3f* normals, int start, vec3f offset, char treshold=0 );
	static int getFloatInterpolatedCube( float verticeValues[8], vec3f* triangles, vec3f* normals, int start, vec3f offset, float maxValue = 1, float treshold=0 );

	static int getCubeIndex( char verticeValues[8], char treshold = 0 );
	
	static void initTexture();
	static void setTexture(GLenum textureSlot);
};

#endif