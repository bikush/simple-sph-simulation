#include "LineGrid.h"
#include <iostream>

void fillData(float * vertice, glm::vec3 position, glm::vec3 color) {
	vertice[0] = position.x;
	vertice[1] = position.y;
	vertice[2] = position.z;
	vertice[3] = color.x;
	vertice[4] = color.y;
	vertice[5] = color.z;
}

void fillElement(GLuint* elements, int index) {
	elements[index] = index;
}

LineGrid::LineGrid(int xCount, float deltaX, float offsetX, int yCount, float deltaY, float offsetY) : Object3D()
{
	int elementCount = (xCount + yCount) * 2;
	int vertexCount = elementCount * 6;
	GLfloat* vertices = new GLfloat[ vertexCount ];

	float endX = offsetX * 2 + deltaX * (xCount - 1);
	float endY = offsetY * 2 + deltaY * (yCount - 1);
	for (int i = 0; i < xCount; i++) {
		float x = offsetX + deltaX * i;
		fillData(&vertices[(i * 2) * 6],	{ x,0.0f,0.0f }, { x/endX,1.0f,1.0f });	// Start
		fillData(&vertices[(i * 2 +1) * 6],	{ x,endY,0.0f }, { 1.0f,1.0f,1.0f });	// End
	}

	int indexOffset = xCount * 2 * 6;
	int elementIndexOffset = xCount * 2;
	for (int i = 0; i < yCount; i++) {
		float y = offsetY + deltaY * i;
		fillData(&vertices[indexOffset + (i * 2) * 6],		{ 0.0f,y,0.0f }, { 1.0f,y/endY,1.0f });	// Start
		fillData(&vertices[indexOffset + (i * 2 + 1) * 6],	{ endX,y,0.0f }, { 1.0f,1.0f,1.0f });	// End
	}	

	GLuint* elements = new GLuint[elementCount];
	for (int i = 0; i < elementCount; i++) {
		elements[i] = i;
	}

	initializeVBO(vertices, vertexCount * sizeof(GLfloat), elements, elementCount * sizeof(GLuint), GL_LINES);
	
	delete[] vertices;
	delete[] elements;
}

LineGrid::~LineGrid()
{
}
