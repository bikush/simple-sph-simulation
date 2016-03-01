#pragma once
#ifndef _MARCHING_CUBES_H
#define _MARCHING_CUBES_H

#include "Vector3D.h"
#include <GL\glew.h>
class MappedData;
class ShaderProgram;

/*
	Visualisation of weighed grid data using Marching Cubes algorithm. Generates triangles
	using MarchingCubesFactory class. Data weight is one byte value, used to offset triangle
	vertices.
	
	Improvments: space partitioning for speed increase (octree?), triangle memory management	
*/
class MarchingCubes
{
	static const int TRIANGLE_COUNT_INCREASE = 1024;
	static const int DATA_CHANGE_DIV = 8;
	static const int DATA_CHANGE_SIZE = DATA_CHANGE_DIV*DATA_CHANGE_DIV*DATA_CHANGE_DIV;

	char* dataField;
	char dataMax;
	char treshold;
	int dataWidth;
	int dataHeight;
	int dataDepth;
	int dataSize;

	bool* dataChanged;
	int dataChangedWidth;
	int dataChangedHeight;
	int dataChangedDepth;
	bool dataChangedGlobal;


	bool useInterpolated;

	vec3f position;
	vec3f scale;

	vec3f* triangles;
	vec3f* normals;
	int trianglesCount;
	int trianglesSize;
		
	GLuint texData;
	vec3f* pointGrid;
	GLuint pointGridBuffer;

	void generateTriangles();
	void generateTriangles( int x, int countX, int y, int countY, int z, int countZ );
	void drawTriangleBuffer();

	void setDataChanged( int x, int y, int z, bool value = true );
	bool getDataChanged( int x, int y, int z );
	void setAllDataChanged( bool value );

public:
	MarchingCubes( );
	MarchingCubes( const char* filePath );
	MarchingCubes( int w, int h, int d, char maxValue, vec3f pos = vec3f(0,0,0), vec3f scl = vec3f(1,1,1) );
	~MarchingCubes( );

	void setUseInterpolated( bool use )
	{
		useInterpolated = use;
	}

	void set( int x, int y, int z, float value );
	//void set( int x, int y, int z, char value );
	void flip( int x, int y, int z );
	void clear( );

	void drawGrid( vec3f colorFalse = vec3f(1,0,0), vec3f colorTrue = vec3f(0,1,0) );	
	void drawColoredCubes( vec3f color );
	void drawLightedCubes( GLfloat material[4] );	
	
	void putSphere(float x, float y, float z, float r );

	vec3f getDataDimensions();
	char* getData();
	int increaseTreshold();

	void initPointGrid();
	void initTextures();
	void setTextures();	
	void initUniforms( GLuint program );
	void drawWithShader( ShaderProgram* shader );
};


#endif