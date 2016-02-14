#pragma once
#ifndef POINT_DATA_VISUALISER_H
#define POINT_DATA_VISUALISER_H

#include "Vector3D.h"
#include <GL\glew.h>
class ShaderProgram;
class Frustum;

class PointDataVisualiser
{
	vec3f* buffer;
	int bufferElements;
	int bufferSize;

	ShaderProgram* shader;
	GLuint bufferID;
	bool useShader;

	GLuint vbo;
	GLuint vao;

	GLuint textureID;

	vec3f position;
	vec3f scale;
	vec3f color;

	float size;

	void drawPipeline();	
	void drawShaded();
	
public:
	PointDataVisualiser( const char* texturePath = 0, bool useShader = false );
	~PointDataVisualiser();

	void setImage( const char* texturePath );
	void setPosition( float x, float y, float z );
	void setScale( float sx, float sy, float sz );
	void setColor( float r, float g, float b );

	void clearBuffer();
	void pushPoint( vec3f point );
	void pushPoint( float x, float y, float z );

	void draw( bool forcePipeline = false );
	void drawPoints();
	
	void drawArray();	

	void setPointSize( float size );
	float getPointSize();
};

#endif