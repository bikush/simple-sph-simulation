#pragma once
#ifndef MARCHING_CUBES_SHADED_H
#define MARCHING_CUBES_SHADED_H

#include "Vector3D.h"
#include "Transform.h"
#include <GL\glew.h>

class ShaderProgram;
class Camera;

class MarchingCubesShaded{
	static const int DATA_MIN = 8;
	
	float* dataField;
	float treshold;
	float dataMax;
	int dataWidth;
	int dataHeight;
	int dataDepth;
	int dataSize;
	bool dataChanged;
	
	vec3f position;
	vec3f span;
	vec3f dSpan;
	
	void initGridBuffer();
	void initDataField();
	
	ShaderProgram* mcShader;

	GLuint gridVao;
	float* grid;
	GLuint gridHandle;
	vec3f gridStep;
	GLint gridSize;

	GLuint dataTexID;

	GLfloat testMC_treshold;
	GLint testMC_dim;
		
	void setUnchecked( int x, int y, int z, float value );

public:
	MarchingCubesShaded( const char* filePath );
	~MarchingCubesShaded( );

	Transform transform;
	
	void set( int x, int y, int z, float value );
	void set( int position, float value );
	void clear( );

	void draw( const Camera& frustum );
	
	void putSphere( float x, float y, float z, float r );

	vec3f getScale();
	vec3f getPosition();	

	float getTreshold();
	void setTreshold(float);
};

#endif