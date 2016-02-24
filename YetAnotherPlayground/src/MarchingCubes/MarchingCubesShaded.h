#pragma once
#ifndef MARCHING_CUBES_SHADED_H
#define MARCHING_CUBES_SHADED_H

#include "Vector3D.h"
#include "Transform.h"
#include <GL\glew.h>

class ShaderProgram;
class Camera;

// Draws a mesh dynamically from a grid of discrete weights.
// Utilizes the power of the geometry shader.
// Far from optimal, but usable.
class MarchingCubesShaded{
private:
	static const int DATA_MIN = 8;
	
	float* dataField;
	float treshold;
	float dataMax;
	int dataWidth;
	int dataHeight;
	int dataDepth;
	int dataSize;

	GLuint dataTexID;
	bool dataChanged;
	
	vec3f vPosition;	// Origin of the virtual space where MarchingCubes are generated.
	vec3f vSpan;		// Dimensions of the virtual space where MarchingCubes are generated.
	vec3f deltaSpan;	// Per data virtual grid size.
	
	void initGridBuffer();
	void initDataField();
	
	ShaderProgram* mcShader;

	GLuint gridVao;
	GLuint gridHandle;
	vec3f gridStep;
	GLint gridElementCount;

		
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