#pragma once
#ifndef MARCHING_CUBES_SHADED_H
#define MARCHING_CUBES_SHADED_H

#include "GlmVec.h"
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
	
	glm::vec3 vPosition;	// Origin of the virtual space where MarchingCubes are generated.
	glm::vec3 vSpan;		// Dimensions of the virtual space where MarchingCubes are generated.
	glm::vec3 deltaSpan;	// Per data virtual grid size.
	
	void initGridBuffer();
	void initDataField();
	
	ShaderProgram* mcShader;

	GLuint gridVao;
	GLuint gridHandle;
	glm::vec3 gridStep;
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

	glm::vec3 getScale();
	glm::vec3 getPosition();	

	float getTreshold();
	void setTreshold(float);
};

#endif