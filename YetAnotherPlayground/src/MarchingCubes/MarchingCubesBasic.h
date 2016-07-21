#pragma once
#ifndef _MARCHING_CUBES_BASIC_H
#define _MARCHING_CUBES_BASIC_H

#include "GlmVec.h"
#include <gl\glew.h>
class MappedData;
class ShaderProgram;

/*
	Visualisation of weighed grid data using Marching Cubes algorithm. Generates triangles
	using MarchingCubesBasicFactory class. Data weight is one byte value, used to offset triangle
	vertices.
	
	Improvments: space partitioning for speed increase (octree?), triangle memory management	
*/
class MarchingCubesBasic
{
	static const int TRIANGLE_COUNT_INCREASE = 1024;
	static const int DATA_CHANGE_DIV = 8;
	static const int DATA_CHANGE_SIZE = DATA_CHANGE_DIV*DATA_CHANGE_DIV*DATA_CHANGE_DIV;

	float* dataField;
	float treshold;
	float dataMax;
	int dataWidth;
	int dataHeight;
	int dataDepth;
	int dataSize;
	bool dataChanged;
	
	glm::vec3 position;
	glm::vec3 span;
	glm::vec3 dSpan;
	
	glm::vec3* triangles;
	glm::vec3* normals;
	int trianglesCount;
	int trianglesSize;
		
	void setUnchecked( int x, int y, int z, float value );
	void generateTriangles();
//	void generateTriangles( int x, int countX, int y, int countY, int z, int countZ );
	void drawTriangleBuffer();

public:
	MarchingCubesBasic( );
	MarchingCubesBasic( const char* filePath );
	MarchingCubesBasic( int w, int h, int d, float treshold = 0.0f, 
						float dataMax = 1.0f, glm::vec3 position = glm::vec3(0,0,0), glm::vec3 span = glm::vec3(1,1,1) );
	~MarchingCubesBasic( );
	
	void set( int x, int y, int z, float value );
	void set( int position, float value );
	void clear( );

	void drawGrid( glm::vec3 colorBelow = glm::vec3(1,0,0), glm::vec3 colorAbove = glm::vec3(0,1,0) );	
	void drawColoredCubes( glm::vec3 color );
	void drawLightedCubes( GLfloat material[4] );
	
	void putSphere( float x, float y, float z, float r );

	glm::vec3 getScale();
	glm::vec3 getPosition();	
};


#endif