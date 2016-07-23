#pragma once
#ifndef _MARCHING_CUBES_FACTORY_H
#define _MARCHING_CUBES_FACTORY_H

#include "GlmVec.h"
#include <GL\glew.h>

class MarchingCubesFactory
{	
	static GLuint texEdgeTable;
	static GLuint texTriTable;

	static glm::vec3 cubeVertices[8];	// cube vertices
	static glm::vec3 cubeEdges[12];	// cube edges
	static glm::vec3 cubeEdgesOffset[12];	// cube edges offset direction
	
		
public:
	static const int MAX_VERTICES = 15;
	static int triTable[256][16];
	static int edgeTable[256];
		
	static int getCube( int cubeIndex, glm::vec3* triangles, glm::vec3* normals, int start, glm::vec3 offset );
	static int getInterpolatedCube( char verticeValues[8], char maxValue, glm::vec3* triangles, glm::vec3* normals, int start, glm::vec3 offset, char treshold=0 );
	static int getFloatInterpolatedCube( float verticeValues[8], glm::vec3* triangles, glm::vec3* normals, int start, glm::vec3 offset, float maxValue = 1, float treshold=0 );

	static int getCubeIndex( char verticeValues[8], char treshold = 0 );
	
	static void initTexture();
	static void setTexture(GLenum textureSlot);
};

#endif