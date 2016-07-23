#include "LineGrid.h"
#include <iostream>

void fillData(float * vertice, glm::vec3 position, glm::vec3 color) 
{
	vertice[0] = position.x;
	vertice[1] = position.y;
	vertice[2] = position.z;
	vertice[3] = color.x;
	vertice[4] = color.y;
	vertice[5] = color.z;
}

void fillElement(GLuint* elements, int index) 
{
	elements[index] = index;
}

LineGrid::LineGrid(int xCount, float deltaX, float offsetX, int yCount, float deltaY, float offsetY) : 
	Object3D(), lineWidth(1.0f)
{
	int elementCount = (xCount + yCount) * 2;
	int vertexCount = elementCount * 6;
	GLfloat* vertices = new GLfloat[ vertexCount ];

	float endX = offsetX * 2 + deltaX * (xCount - 1);
	float endY = offsetY * 2 + deltaY * (yCount - 1);
	for (int i = 0; i < xCount; i++) {
		float x = offsetX + deltaX * i;
		fillData(&vertices[(i * 2) * 6],	{ x,0.0f,0.0f }, { 0.0f,x/endX,1.0f });	// Start
		fillData(&vertices[(i * 2 +1) * 6],	{ x,endY,0.0f }, { 0.0f,1.0f,1.0f });	// End
	}

	int indexOffset = xCount * 2 * 6;
	int elementIndexOffset = xCount * 2;
	for (int i = 0; i < yCount; i++) {
		float y = offsetY + deltaY * i;
		fillData(&vertices[indexOffset + (i * 2) * 6],		{ 0.0f,y,0.0f }, { 1.0f,y/endY,0.0f });	// Start
		fillData(&vertices[indexOffset + (i * 2 + 1) * 6],	{ endX,y,0.0f }, { 1.0f,1.0f,0.0f });	// End
	}	

	GLuint* elements = new GLuint[elementCount];
	for (int i = 0; i < elementCount; i++) {
		elements[i] = i;
	}

	initializeVBO(vertices, vertexCount * sizeof(GLfloat), elements, elementCount * sizeof(GLuint), GL_LINES);
	
	delete[] vertices;
	delete[] elements;
}

LineGrid::LineGrid( float length, float lineWidth ) : 
	Object3D(), lineWidth(lineWidth)
{
	int elementCount = 18;
	int vertexCount = 12 * 6;
	GLfloat* vertices = new GLfloat[vertexCount];
	GLuint* elements = new GLuint[elementCount];

	float arrowEnd = length * 0.9f;
	float arrowOffset = length * 0.07f;

	// x-Axis
	fillData(&vertices[0], { -length,0.0f,0.0f }, { 1.0,0.0f,0.0f });	
	fillData(&vertices[1*6], { length,0.0,0.0f }, { 1.0f,0.0f,0.0f });	
	fillData(&vertices[2*6], { arrowEnd,0.0f,arrowOffset }, { 1.0,0.0f,0.0f });
	fillData(&vertices[3*6], { arrowEnd,0.0f,-arrowOffset }, { 1.0f,0.0f,0.0f });

	elements[0] = 0;
	elements[1] = 1;
	elements[2] = 1;
	elements[3] = 2;
	elements[4] = 1;
	elements[5] = 3;
	
	// y-Axis
	fillData(&vertices[4*6], { 0.0f,-length,0.0f }, { 0.0,1.0f,0.0f });
	fillData(&vertices[5*6], { 0.0f,length,0.0f }, { 0.0f,1.0f,0.0f });
	fillData(&vertices[6*6], { arrowOffset,arrowEnd,0.0f }, { 0.0,1.0f,0.0f });
	fillData(&vertices[7*6], { -arrowOffset,arrowEnd,0.0f }, { 0.0f,1.0f,0.0f });

	elements[6] = 4;
	elements[7] = 5;
	elements[8] = 5;
	elements[9] = 6;
	elements[10] = 5;
	elements[11] = 7;

	// z-Axis
	fillData(&vertices[8*6], { 0.0f,0.0f,-length }, { 0.0,0.0f,1.0f });
	fillData(&vertices[9*6], { 0.0f,0.0f,length }, { 0.0f,0.0f,1.0f });
	fillData(&vertices[10*6], { 0.0f,arrowOffset,arrowEnd }, { 0.0,0.0f,1.0f });
	fillData(&vertices[11*6], { 0.0f,-arrowOffset,arrowEnd }, { 0.0f,0.0f,1.0f });

	elements[12] = 8;
	elements[13] = 9;
	elements[14] = 9;
	elements[15] = 10;
	elements[16] = 9;
	elements[17] = 11;

	initializeVBO(vertices, vertexCount * sizeof(GLfloat), elements, elementCount * sizeof(GLuint), GL_LINES);

	delete[] vertices;
	delete[] elements;
}

LineGrid::~LineGrid()
{
}

void LineGrid::draw(const glm::mat4 & viewProjection)
{
	glLineWidth(lineWidth);
	Object3D::draw(viewProjection);
	glLineWidth(1.0f);
}
