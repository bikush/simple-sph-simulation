#pragma once
#ifndef _H_Shape
#define _H_Shape

#include <GL/glew.h>
#include <glm/gtc/matrix_transform.hpp>

class ShaderProgram;

class Shape
{
public:
	Shape();
	~Shape();
	static Shape* CreateShape( const glm::vec2& startPosition, const glm::vec2& startScale );

	void draw( const glm::mat4& viewProjection );
	void setPosition( const glm::vec2& newPosition );
	void setPosition(const glm::vec3& newPosition);
	void setScale( const glm::vec2& newScale );
	void setScale( const float newScale );

	glm::vec2 getPosition();
	glm::vec2 getScale();

private:
	ShaderProgram* program;
	GLuint vbo_triangle;
	GLuint vbo_elements;
	GLint attribute_coord2d;
	GLint attribute_v_color;

	glm::vec3 position;
	glm::vec3 scale;
	bool recalculate;
	glm::mat4 modelMatrix;

	void setupModelMatrix();
};



#endif