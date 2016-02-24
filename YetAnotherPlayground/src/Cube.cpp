#include "Cube.h"

Cube::Cube() : Object3D()
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

	initializeVBO(triangle_attributes, sizeof(triangle_attributes), elements, sizeof(elements), GL_TRIANGLES);
}

Cube::~Cube()
{
}

Cube * Cube::CreateCube(const glm::vec3 & startPosition, const glm::vec3 & startScale)
{
	Cube* cube = new Cube();
	cube->transform.setPosition(startPosition);
	cube->transform.setScale(startScale);
	return cube;
}

