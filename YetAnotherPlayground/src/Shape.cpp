#include "Shape.h"
#include "ShaderProgram.h"

#include <glm/gtc/type_ptr.hpp>
#include <glm/mat4x4.hpp>

Shape::Shape()
{
	/*GLfloat triangle_attributes[] = {
	-0.5f,  0.5f, 1.0f, 0.0f, 0.0f, // Top-left
     0.5f,  0.5f, 0.0f, 1.0f, 0.0f, // Top-right
     0.5f, -0.5f, 0.0f, 0.0f, 1.0f, // Bottom-right
    -0.5f, -0.5f, 1.0f, 1.0f, 1.0f  // Bottom-left
	};*/
	GLfloat triangle_attributes[] = {
		0.0f, 1.0f, 1.0f, 0.0f, 0.0f, // Top-left
		1.0f, 1.0f, 0.0f, 1.0f, 0.0f, // Top-right
		1.0f, 0.0f, 0.0f, 0.0f, 1.0f, // Bottom-right
		0.0f, 0.0f, 1.0f, 1.0f, 1.0f  // Bottom-left
	};
	glGenBuffers(1, &vbo_triangle);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_triangle);
	glBufferData(GL_ARRAY_BUFFER, sizeof(triangle_attributes), triangle_attributes, GL_STATIC_DRAW);

	GLuint elements[] = {
		0, 1, 2,
		2, 3, 0
	};
	glGenBuffers(1, &vbo_elements);	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo_elements);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements), elements, GL_STATIC_DRAW);	

	program = ShaderProgram::CreateBaseShader();
	attribute_v_color = program->getAttribute("v_color");
	attribute_coord2d = program->getAttribute("coord2d");

	position = glm::vec3(0,0,0);
	scale = glm::vec3(1,1,1);
	setupModelMatrix();
}

Shape::~Shape()
{
	if( program )
	{
		//delete program;
	}
	glDeleteBuffers(1, &vbo_triangle);
}

Shape* Shape::CreateShape( const glm::vec2& startPosition, const glm::vec2& startScale )
{
	Shape* shape = new Shape();
	shape->setPosition( startPosition );
	shape->setScale( startScale );
	return shape;
}

void Shape::setPosition( const glm::vec2& newPosition )
{
	position.x = newPosition.x;
	position.y = newPosition.y;
	recalculate = true;
}

void Shape::setPosition(const glm::vec3& newPosition)
{
	position = newPosition;
	recalculate = true;
}

void Shape::setScale( const glm::vec2& newScale )
{
	scale.x = newScale.x;
	scale.y = newScale.y;
	recalculate = true;
}

void Shape::setScale( const float newScale )
{
	scale.x = newScale;
	scale.y = newScale;
	recalculate = true;
}

glm::vec2 Shape::getPosition()
{
	return glm::vec2( position.x, position.y );
}

glm::vec2 Shape::getScale()
{
	return glm::vec2( scale.x, scale.y );
}

void Shape::setupModelMatrix()
{
	//modelMatrix = glm::translate( glm::scale( glm::mat4(1.0f), scale ), position );
	modelMatrix = glm::scale( glm::translate( glm::mat4(1.0f), position ), scale );
	recalculate = false;
}

void Shape::draw( const glm::mat4& viewProjection )
{
	if( recalculate )
	{
		setupModelMatrix();
	}

	glm::mat4 MVP = viewProjection * modelMatrix;

	program->turnOn();
	program->setUniformM4("mvp", glm::value_ptr(MVP));

	glEnableVertexAttribArray(attribute_coord2d);
	glEnableVertexAttribArray(attribute_v_color);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_triangle);
	glVertexAttribPointer(
		attribute_coord2d,   // attribute
		2,                   // number of elements per vertex, here (x,y)
		GL_FLOAT,            // the type of each element
		GL_FALSE,            // take our values as-is
		5 * sizeof(GLfloat), // next coord2d appears every 5 floats
		0                    // offset of the first element
	);
	glVertexAttribPointer(
		attribute_v_color,      // attribute
		3,                      // number of elements per vertex, here (r,g,b)
		GL_FLOAT,               // the type of each element
		GL_FALSE,               // take our values as-is
		5 * sizeof(GLfloat),    // next color appears every 5 floats
		(GLvoid*) (2 * sizeof(GLfloat))  // offset of first element
	);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo_elements);
	glDrawElements(GL_TRIANGLES,6, GL_UNSIGNED_INT, 0);

	glDisableVertexAttribArray(attribute_coord2d);
	glDisableVertexAttribArray(attribute_v_color);
}