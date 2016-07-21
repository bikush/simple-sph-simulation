#include "PointDataVisualiser.h"
#include "TextureManager.h"
#include "ShaderProgram.h"
#include "Camera.h"
#include "Utility.h"
#include <iostream>
#include <glm\gtc\type_ptr.hpp>
#include <glm\gtc\matrix_transform.hpp>

#define BUFFER_SIZE_INC 64

PointDataVisualiser::PointDataVisualiser( const char* texturePath, bool useShader ) :
	bufferSize(BUFFER_SIZE_INC), buffer(new glm::vec3[BUFFER_SIZE_INC]), bufferElements(0),
	textureID(0), bufferID(0), color({1.f,1.f,1.f}),
	shader(nullptr), useShader(useShader && GLEW_ARB_geometry_shader4 == GL_TRUE),
	pointSize(5.0f)
{
	setImage( texturePath );

	// Setup vao and vbo connections
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	// NULL => do not copy the buffer
	glBufferData(GL_ARRAY_BUFFER, bufferSize * sizeof(float) * 3, NULL, GL_DYNAMIC_DRAW);

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (GLubyte *)NULL);
		
	if( useShader )
	{
		shader = ShaderProgram::CreateShader(
			"data/shaders/testPS.vert",
			"data/shaders/testPS.geom",
			"data/shaders/testPS.frag",
			true );	
	}
}

PointDataVisualiser::~PointDataVisualiser()
{
	delete [] buffer;
	glDeleteBuffers(1, &vbo);
	glDeleteVertexArrays(1, &vao);
}

void PointDataVisualiser::setImage( const char* texturePath )
{
	if( texturePath == nullptr )
	{
		textureID = 0;
		return;
	}
	textureID = TextureManager::instance.loadTexture( texturePath );
}

void PointDataVisualiser::setColor( float r, float g, float b )
{
	color = glm::vec3( r, g, b );
}

void PointDataVisualiser::clearBuffer()
{
	bufferElements = 0;
}

void PointDataVisualiser::pushPoint( glm::vec3 point )
{
	pushPoint( point.x, point.y, point.z );
}

void PointDataVisualiser::pushPoint( float x, float y, float z )
{
	bool resize = bufferElements+1 > bufferSize;
	if( resize )
	{
		int newSize = bufferSize + BUFFER_SIZE_INC;
		glm::vec3 *newBuffer = new glm::vec3[ newSize ];
		memcpy_s( newBuffer, newSize*sizeof(glm::vec3), buffer, bufferElements*sizeof(glm::vec3) );
		delete [] buffer;
		buffer = newBuffer;
		bufferSize = newSize;

		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		// NULL => do not copy the buffer
		glBufferData(GL_ARRAY_BUFFER, bufferSize * sizeof(float) * 3, NULL, GL_DYNAMIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
	buffer[ bufferElements ] = glm::vec3( x, y, z );
	bufferElements++;
}

void PointDataVisualiser::drawArray()
{	
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferSubData(GL_ARRAY_BUFFER, 0, bufferSize * sizeof(float) * 3, buffer );
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(vao);
	glDrawArrays(GL_POINTS, 0, bufferElements );/**/
	glBindVertexArray(0);
}

void PointDataVisualiser::drawShaded(const Camera& camera)
{
	glDisable(GL_CULL_FACE);
	
	auto projection = camera.getProjection();
	auto modelView = camera.getView() * transform.getTransformMatrix();
	auto MVP = projection * modelView;

	auto up = camera.getUp();
	
	shader->turnOn();
	shader->setUniformM4( "ModelViewMatrix", glm::value_ptr(modelView) );
	shader->setUniformM4( "ProjectionMatrix", glm::value_ptr(projection) );
	shader->setUniformM4("mvp", glm::value_ptr(MVP));
	shader->setUniformV3("up", up.x, up.y, up.z);
	shader->setUniformF("Size2", pointSize);
	shader->setUniformI("SpriteTex", 0 );
	shader->setUniformV3("Color", color.x, color.y, color.z );
	
	drawArray();
	shader->turnOff();	

	glEnable(GL_CULL_FACE);	
}

// TODO: remove or fix pipeline draw
void PointDataVisualiser::drawPipeline(const Camera& camera)
{
	float quadratic[] =  { 0.4f, 0.01f, 0.0f };
	glPointParameterfv( GL_POINT_DISTANCE_ATTENUATION, quadratic );

	float maxSize = 0.0f;
	glGetFloatv( GL_POINT_SIZE_MAX, &maxSize );
	if( maxSize > 100 )
		maxSize = 100;
	glPointSize( fmaxf( pointSize * 10 , maxSize ) );

	glPointParameterf( GL_POINT_FADE_THRESHOLD_SIZE, 60.0f );
	glPointParameterf( GL_POINT_SIZE_MIN, 1.0f );
	glPointParameterf( GL_POINT_SIZE_MAX, maxSize );

	glColor3f( color.x, color.y, color.z );
	glTexEnvf( GL_POINT_SPRITE, GL_COORD_REPLACE, GL_TRUE );
	
	glEnable( GL_POINT_SPRITE );		
		drawArray();
	glDisable( GL_POINT_SPRITE );
}

void PointDataVisualiser::draw(const Camera& camera, bool forcePipeline )
{	
	ShaderProgram::turnOff();
	glEnable(GL_TEXTURE_2D);
	glActiveTexture( GL_TEXTURE0 );
	glBindTexture( GL_TEXTURE_2D, textureID );

		GLboolean blendEnabled = glIsEnabled( GL_BLEND );
		glEnable( GL_BLEND );
		glBlendFunc(GL_ONE, GL_ONE);
		glDepthMask(GL_FALSE);
			
			if( useShader && !forcePipeline )
				drawShaded(camera);
			else
				drawPipeline(camera);

		glDepthMask(GL_TRUE);
		if( !blendEnabled ) glDisable( GL_BLEND );

	glDisable(GL_TEXTURE_2D);		
}

void PointDataVisualiser::drawPoints()
{
	glPointSize( pointSize );
	drawArray();
}

void PointDataVisualiser::setPointSize( float size )
{
	pointSize = size;
}

float PointDataVisualiser::getPointSize()
{
	return pointSize;
}
