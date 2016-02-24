#pragma once
#include <glm\vec3.hpp>
#include <glm\mat4x4.hpp>
#include <GL\glew.h>

#include "Transform.h"

class ShaderProgram;

// TODO: Instanced objects
// TODO: Shared vao for the same object, eg. a cube, to reduce mem. footprint

class Object3D
{
public:
	Object3D();
	virtual ~Object3D();

	void draw(const glm::mat4& viewProjection);
	
	Transform transform;

protected:
	void initializeVBO(const float* triangleAttributes, int attributeCount, const GLuint * elements, int elementCount, GLenum mode);

private:
	ShaderProgram* program;
	GLuint vbo_triangle;
	GLuint vbo_elements;
	GLuint vao;
	GLint attribute_position;
	GLint attribute_v_color;
	GLenum draw_mode;
	int elementCount;
};