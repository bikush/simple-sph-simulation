#include "Object3D.h"

#include "ShaderProgram.h"

#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>


Object3D::Object3D() : transform()
{
	program = ShaderProgram::CreateBase3DShader();
	attribute_v_color = program->getAttribute("v_color");
	attribute_position = program->getAttribute("position");

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

	// Vertex attribute setup
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	//glBindBuffer(GL_ARRAY_BUFFER, vbo_triangle); // already bound

	// Vertex data buffer
	glGenBuffers(1, &vbo_triangle);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_triangle);
	glBufferData(GL_ARRAY_BUFFER, attributeCount, triangleAttributes, GL_STATIC_DRAW);

	glEnableVertexArrayAttrib(vao, attribute_position);
	glVertexAttribPointer(
		attribute_position,  // attribute
		3,                   // number of elements per vertex, here (x,y,z)
		GL_FLOAT,            // the type of each element
		GL_FALSE,            // take our values as-is
		6 * sizeof(GLfloat), // next coord2d appears every 6 floats
		0                    // offset of the first element
		);

	glEnableVertexArrayAttrib(vao, attribute_v_color);
	glVertexAttribPointer(
		attribute_v_color,      // attribute
		3,                      // number of elements per vertex, here (r,g,b)
		GL_FLOAT,               // the type of each element
		GL_FALSE,               // take our values as-is
		6 * sizeof(GLfloat),    // next color appears every 5 floats
		(GLvoid*)(3 * sizeof(GLfloat))  // offset of first element
		);

	// Element index buffer
	glGenBuffers(1, &vbo_elements);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo_elements);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, elementCount, elements, GL_STATIC_DRAW);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

}

Object3D::~Object3D()
{
	// TODO: could some objects have a single verticeAttribute and element buffer?
	glDeleteBuffers(1, &vbo_triangle);
	glDeleteBuffers(1, &vbo_elements);
	glDeleteVertexArrays(1, &vao);
}


void Object3D::draw(const glm::mat4 & viewProjection)
{
	if (elementCount & vbo_elements & vbo_triangle == 0)
	{
		return;
	}
	
	glm::mat4 MVP = viewProjection * transform.getTransformMatrix();

	program->turnOn();
	program->setUniformM4("mvp", glm::value_ptr(MVP));
	
	glBindVertexArray(vao);
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo_elements);
	glDrawElements(draw_mode, elementCount, GL_UNSIGNED_INT, 0);
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}
