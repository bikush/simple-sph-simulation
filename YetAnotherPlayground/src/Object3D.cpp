#include "Object3D.h"

#include "ShaderProgram.h"
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>


Object3D::Object3D()
{
	program = ShaderProgram::CreateBase3DShader();
	attribute_v_color = program->getAttribute("v_color");
	attribute_position = program->getAttribute("position");

	position = glm::vec3(0, 0, 0);
	scale = glm::vec3(1, 1, 1);
	setupModelMatrix();

	vbo_triangle = GL_ZERO;
	vbo_elements = GL_ZERO;
	elementCount = 0;
	draw_mode = GL_TRIANGLES;
}

void Object3D::initializeVBO(const float * triangleAttributes, int attributeCount, const GLuint * elements, int elementCount, GLenum mode)
{
	if (this->elementCount & vbo_elements & vbo_triangle != 0)
	{
		return;
	}

	this->elementCount = elementCount;
	draw_mode = mode;

	glGenBuffers(1, &vbo_triangle);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_triangle);
	glBufferData(GL_ARRAY_BUFFER, attributeCount, triangleAttributes, GL_STATIC_DRAW);

	glGenBuffers(1, &vbo_elements);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo_elements);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, elementCount, elements, GL_STATIC_DRAW);
}

Object3D::~Object3D()
{
	glDeleteBuffers(1, &vbo_triangle);
	// TODO: delete elements buffer?
}


void Object3D::draw(const glm::mat4 & viewProjection)
{
	if (elementCount & vbo_elements & vbo_triangle == 0)
	{
		return;
	}
	if (recalculate)
	{
		setupModelMatrix();
	}

	glm::mat4 MVP = viewProjection * modelMatrix;

	program->turnOn();
	program->setUniformM4("mvp", glm::value_ptr(MVP));

	glEnableVertexAttribArray(attribute_position);
	glEnableVertexAttribArray(attribute_v_color);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_triangle);
	glVertexAttribPointer(
		attribute_position,  // attribute
		3,                   // number of elements per vertex, here (x,y,z)
		GL_FLOAT,            // the type of each element
		GL_FALSE,            // take our values as-is
		6 * sizeof(GLfloat), // next coord2d appears every 6 floats
		0                    // offset of the first element
		);
	glVertexAttribPointer(
		attribute_v_color,      // attribute
		3,                      // number of elements per vertex, here (r,g,b)
		GL_FLOAT,               // the type of each element
		GL_FALSE,               // take our values as-is
		6 * sizeof(GLfloat),    // next color appears every 5 floats
		(GLvoid*)(3 * sizeof(GLfloat))  // offset of first element
		);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo_elements);
	glDrawElements(draw_mode, elementCount, GL_UNSIGNED_INT, 0);

	glDisableVertexAttribArray(attribute_position);
	glDisableVertexAttribArray(attribute_v_color);
}

void Object3D::setPosition(const glm::vec3 & newPosition)
{
	position = newPosition;
	recalculate = true;
}

void Object3D::setScale(const glm::vec3 & newScale)
{
	scale = newScale;
	recalculate = true;
}

void Object3D::setScale(const float newScale)
{
	setScale({ newScale, newScale, newScale });
}

glm::vec3 Object3D::getPosition()
{
	return position;
}

glm::vec3 Object3D::getScale()
{
	return scale;
}

void Object3D::setupModelMatrix()
{
	modelMatrix = glm::scale(glm::translate(glm::mat4(1.0f), position), scale);
	recalculate = false;
}