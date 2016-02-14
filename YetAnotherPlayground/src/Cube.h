#pragma once

#include <glm\vec3.hpp>
#include <glm\mat4x4.hpp>
#include <GL\glew.h>

class ShaderProgram;

class Cube
{
public:
	Cube();
	~Cube();
	static Cube* CreateCube(const glm::vec3& startPosition, const glm::vec3& startScale);

	void draw(const glm::mat4& viewProjection);
	void setPosition(const glm::vec3& newPosition);
	void setScale(const glm::vec3& newScale);
	void setScale(const float newScale);

	glm::vec3 getPosition();
	glm::vec3 getScale();

private:
	ShaderProgram* program;
	GLuint vbo_triangle;
	GLuint vbo_elements;
	GLint attribute_position;
	GLint attribute_v_color;

	glm::vec3 position;
	glm::vec3 scale;
	bool recalculate;
	glm::mat4 modelMatrix;

	void setupModelMatrix();
};

