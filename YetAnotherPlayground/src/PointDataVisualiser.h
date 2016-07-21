#pragma once
#ifndef POINT_DATA_VISUALISER_H
#define POINT_DATA_VISUALISER_H

#include "GlmVec.h"
#include "Transform.h"
#include <GL\glew.h>

class ShaderProgram;
class Camera;

class PointDataVisualiser
{
	glm::vec3* buffer;
	int bufferElements;
	int bufferSize;

	ShaderProgram* shader;
	GLuint bufferID;
	bool useShader;

	GLuint vbo;
	GLuint vao;

	GLuint textureID;

	glm::vec3 color;

	float pointSize;

	void drawShaded(const Camera& camera);
	
public:
	PointDataVisualiser( const char* texturePath = 0, bool useShader = false );
	~PointDataVisualiser();

	void setImage( const char* texturePath );
	void setColor( float r, float g, float b );

	void clearBuffer();
	void pushPoint( glm::vec3 point );
	void pushPoint( float x, float y, float z );

	void draw( const Camera& camera );
	void drawPoints();	
	void drawArray();

	void setPointSize( float size );
	float getPointSize();

	Transform transform;
};

#endif