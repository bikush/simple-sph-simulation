#include "Cube.h"

#include "ShaderProgram.h"
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>

Cube::Cube()
{
	GLfloat triangle_attributes[] = {
		// FRONT
		-0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, // Bottom-left
		 0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, // Bottom-right
		 0.5f,  0.5f, -0.5f, 0.0f, 0.0f, 1.0f, // Top-right
		-0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f, // Top-left
		// BACK
		-0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, // Bottom-left
		 0.5f, -0.5f, 0.5f, 0.0f, 1.0f, 0.0f, // Bottom-right
		 0.5f,  0.5f, 0.5f, 0.0f, 0.0f, 1.0f, // Top-right
		-0.5f,  0.5f, 0.5f, 1.0f, 1.0f, 1.0f // Top-left
	};
	glGenBuffers(1, &vbo_triangle);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_triangle);
	glBufferData(GL_ARRAY_BUFFER, sizeof(triangle_attributes), triangle_attributes, GL_STATIC_DRAW);

	GLuint elements[] = {
		2,1,0,
		0,3,2,

		2,3,7,
		7,6,2,

		2,6,5,
		5,1,2,

		4,0,1,
		1,5,4,

		4,5,6,
		6,7,4,

		4,7,3,
		3,0,4
	};
	glGenBuffers(1, &vbo_elements);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo_elements);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements), elements, GL_STATIC_DRAW);

	program = ShaderProgram::CreateBase3DShader();
	attribute_v_color = program->getAttribute("v_color");
	attribute_position = program->getAttribute("position");

	position = glm::vec3(0, 0, 0);
	scale = glm::vec3(1, 1, 1);
	setupModelMatrix();
}


Cube::~Cube()
{
	glDeleteBuffers(1, &vbo_triangle);
}

Cube * Cube::CreateCube(const glm::vec3 & startPosition, const glm::vec3 & startScale)
{
	Cube* cube = new Cube();
	cube->setPosition(startPosition);
	cube->setScale(startScale);
	return cube;
}

void Cube::draw(const glm::mat4 & viewProjection)
{
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
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

	glDisableVertexAttribArray(attribute_position);
	glDisableVertexAttribArray(attribute_v_color);
}

void Cube::setPosition(const glm::vec3 & newPosition)
{
	position = newPosition;
	recalculate = true;
}

void Cube::setScale(const glm::vec3 & newScale)
{
	scale = newScale;
	recalculate = true;
}

void Cube::setScale(const float newScale)
{
	setScale({ newScale, newScale, newScale });
}

glm::vec3 Cube::getPosition()
{
	return position;
}

glm::vec3 Cube::getScale()
{
	return scale;
}

void Cube::setupModelMatrix()
{
	modelMatrix = glm::scale(glm::translate(glm::mat4(1.0f), position), scale);
	recalculate = false;
}
