#pragma once
#include <glm\vec3.hpp>
#include <glm\mat4x4.hpp>
#include <GL\glew.h>

class ShaderProgram;

class Object3D
{
public:
	Object3D();
	virtual ~Object3D();

	void draw(const glm::mat4& viewProjection);
	void setPosition(const glm::vec3& newPosition);
	void setScale(const glm::vec3& newScale);
	void setScale(const float newScale);
	void setAngles(const float xAxis, const float yAxis, const float zAxis);

	glm::vec3 getPosition();
	glm::vec3 getScale();
	glm::vec3 getAxisAngles();

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

	glm::vec3 position;
	glm::vec3 scale;
	glm::vec3 axisAngles;
	bool recalculate;
	glm::mat4 modelMatrix;

	void setupModelMatrix();
};