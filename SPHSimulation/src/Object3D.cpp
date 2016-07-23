#include "Object3D.h"

#include "ShaderProgram.h"

#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>


Object3D::Object3D() : transform()
{
	program = ShaderProgram::CreateBase3DShader();
	attrVertexColor = program->getAttribute("v_color");
	attrVertexPosition = program->getAttribute("position");

	vboGeometry = GL_ZERO;
	vboElements = GL_ZERO;
	elementCount = 0;
	drawMode = GL_TRIANGLES;
}

void Object3D::initializeVBO(const float * geometryAttributes, int attributeCount, const GLuint * elements, int elementCount, GLenum mode)
{
	if (this->elementCount & vboElements & (vboGeometry != 0))
	{
		return;
	}

	drawMode = mode;

	// Vertex attribute setup
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	//glBindBuffer(GL_ARRAY_BUFFER, vbo_triangle); // already bound

	// Vertex data buffer
	glGenBuffers(1, &vboGeometry);
	glBindBuffer(GL_ARRAY_BUFFER, vboGeometry);
	glBufferData(GL_ARRAY_BUFFER, attributeCount, geometryAttributes, GL_STATIC_DRAW);

	glEnableVertexArrayAttrib(vao, attrVertexPosition);
	glVertexAttribPointer(
		attrVertexPosition,  // attribute
		3,                   // number of elements per vertex, here (x,y,z)
		GL_FLOAT,            // the type of each element
		GL_FALSE,            // take our values as-is
		6 * sizeof(GLfloat), // next coord2d appears every 6 floats
		0                    // offset of the first element
		);

	glEnableVertexArrayAttrib(vao, attrVertexColor);
	glVertexAttribPointer(
		attrVertexColor,      // attribute
		3,                      // number of elements per vertex, here (r,g,b)
		GL_FLOAT,               // the type of each element
		GL_FALSE,               // take our values as-is
		6 * sizeof(GLfloat),    // next color appears every 5 floats
		(GLvoid*)(3 * sizeof(GLfloat))  // offset of first element
		);

	// Element index buffer
	glGenBuffers(1, &vboElements);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboElements);
	glVertexArrayElementBuffer(vao, vboElements);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, elementCount, elements, GL_STATIC_DRAW);

	// Actual number of elements
	this->elementCount = elementCount / sizeof(GLuint);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

}

Object3D::~Object3D()
{
	glDeleteBuffers(1, &vboGeometry);
	glDeleteBuffers(1, &vboElements);
	glDeleteVertexArrays(1, &vao);
}


void Object3D::draw(const glm::mat4 & viewProjection) 
{
	if (elementCount & vboElements & (vboGeometry == 0))
	{
		return;
	}
	
	glm::mat4 MVP = viewProjection * transform.getTransformMatrix();

	program->turnOn();
	program->setUniformM4("mvp", glm::value_ptr(MVP));
	
	glBindVertexArray(vao);
	glDrawElements(drawMode, elementCount, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}
