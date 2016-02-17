#include "PointDataVisualiser.h"
#include "TextureManager.h"
#include "ShaderProgram.h"
#include "Camera.h"
#include <iostream>
#include <glm\gtc\type_ptr.hpp>

#define BUFFER_SIZE_INC 64

PointDataVisualiser::PointDataVisualiser( const char* texturePath,bool useShader )
{
	bufferSize = BUFFER_SIZE_INC;
	buffer = new vec3f[ bufferSize ];	
	bufferElements = 0;

	textureID = 0;
	setImage( texturePath );

	shader = 0;
	bufferID = 0;
	
	position = vec3f(0,0,0);
	scale = vec3f(1,1,1);
	color = vec3f(1,1,1);

	// Setup vao and vbo connections
	/**/
		
	if( useShader && GLEW_ARB_geometry_shader4 == GL_TRUE )
	{
		this->useShader = true;
		std::cout << std::endl << "---- PDV Shader ----" << std::endl;
		shader = ShaderProgram::CreateShader(
			"data/shaders/testPS.vert",
			"data/shaders/testPS.geom",
			"data/shaders/testPS.frag",
			true );
	//	shader->attach();	
	//	shader->link();
		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		// NULL => do not copy the buffer
		glBufferData(GL_ARRAY_BUFFER, bufferSize * sizeof(float) * 3, NULL, GL_DYNAMIC_DRAW);

		glGenVertexArrays( 1, &vao );
		glBindVertexArray(vao);
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 0, (GLubyte *)NULL );

		setPointSize(5.0);
			
		std::cout << "--- PDV Done ---" << std::endl;
		shader->turnOff();
	}else{
		this->useShader = false;
	}
}

PointDataVisualiser::~PointDataVisualiser()
{
	delete [] buffer;
}

void PointDataVisualiser::setImage( const char* texturePath )
{
	if( texturePath == 0 )
	{
		textureID = 0;
		return;
	}
	textureID = TextureManager::getInstance()->loadTexture( texturePath );
}

void PointDataVisualiser::setPosition( float x, float y, float z )
{
	position = vec3f( x, y, z );
}

void PointDataVisualiser::setScale( float sx, float sy, float sz )
{
	scale = vec3f( sx, sy, sz );
}

void PointDataVisualiser::setColor( float r, float g, float b )
{
	color = vec3f( r, g, b );
}

void PointDataVisualiser::clearBuffer()
{
	bufferElements = 0;
}

void PointDataVisualiser::pushPoint( vec3f point )
{
	pushPoint( point.x, point.y, point.z );
}

void PointDataVisualiser::pushPoint( float x, float y, float z )
{
	bool resize = bufferElements+1 > bufferSize;
	if( resize )
	{
		int newSize = bufferSize + BUFFER_SIZE_INC;
		vec3f *newBuffer = new vec3f[ newSize ];
		memcpy_s( newBuffer, newSize*sizeof(vec3f), buffer, bufferElements*sizeof(vec3f) );
		delete [] buffer;
		buffer = newBuffer;
		bufferSize = newSize;

		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		// NULL => do not copy the buffer
		glBufferData(GL_ARRAY_BUFFER, bufferSize * sizeof(float) * 3, NULL, GL_DYNAMIC_DRAW);
	}
	buffer[ bufferElements ] = vec3f( x, y, z );
	bufferElements++;
}

void PointDataVisualiser::drawArray()
{
	/*glEnableClientState( GL_VERTEX_ARRAY );
	glVertexPointer( 3, GL_FLOAT, 0, buffer );
	glDrawArrays(GL_POINTS, 0, bufferElements);
	glDisableClientState( GL_VERTEX_ARRAY );/**/
	/*glBegin( GL_POINTS );
	for(int i = 0; i<bufferElements; i++) glVertex3f( buffer[i].x, buffer[i].y, buffer[i].z );
	glEnd();/**/
	
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferSubData(GL_ARRAY_BUFFER, 0, bufferSize * sizeof(float) * 3, buffer );
	glBindVertexArray(vao);
	glDrawArrays(GL_POINTS, 0, bufferElements );/**/
}

void PointDataVisualiser::drawShaded(const Camera& camera)
{
	glDisable(GL_CULL_FACE);
	
	auto proj = camera.getProjection();
	auto modl = camera.getView();
	
	shader->turnOn();
	shader->setUniformM4( "ModelViewMatrix", glm::value_ptr(modl) );
	shader->setUniformM4( "ProjectionMatrix", glm::value_ptr(proj) );
	shader->setUniformF("Size2", 1 );
	shader->setUniformI("SpriteTex", 0 );
	shader->setUniformV3("Color", color.x, color.y, color.z );
	
	//glBindVertexArray(vao);
	//glDrawArrays(GL_POINTS, 0, bufferElements );
	drawArray();
	shader->turnOff();	

	glEnable(GL_CULL_FACE);	
}

void PointDataVisualiser::drawPipeline(const Camera& camera)
{
/*	float quadratic[] =  { 0.4, 0.01f, 0.0f };
	glPointParameterfv( GL_POINT_DISTANCE_ATTENUATION_ARB, quadratic );

	float maxSize = 0.0f;
	glGetFloatv( GL_POINT_SIZE_MAX_ARB, &maxSize );
	if( maxSize > 100 )
		maxSize = 100;
	glPointSize( size*10 > maxSize ? maxSize : size*10 );

	glPointParameterf( GL_POINT_FADE_THRESHOLD_SIZE_ARB, 60.0f );
	glPointParameterf( GL_POINT_SIZE_MIN_ARB, 1.0f );
	glPointParameterf( GL_POINT_SIZE_MAX_ARB, maxSize );

	glColor3f( color.x, color.y, color.z );
	glTexEnvf( GL_POINT_SPRITE_ARB, GL_COORD_REPLACE_ARB, GL_TRUE );
	
	glEnable( GL_POINT_SPRITE_ARB );		
		drawArray();
	glDisable( GL_POINT_SPRITE_ARB );	*/
	float quadratic[] =  { 0.4, 0.01f, 0.0f };
	glPointParameterfv( GL_POINT_DISTANCE_ATTENUATION, quadratic );

	float maxSize = 0.0f;
	glGetFloatv( GL_POINT_SIZE_MAX, &maxSize );
	if( maxSize > 100 )
		maxSize = 100;
	glPointSize( size*10 > maxSize ? maxSize : size*10 );

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
	glPointSize( 5 );
	drawArray();
}

void PointDataVisualiser::setPointSize( float size )
{
	this->size = size*10;
}

float PointDataVisualiser::getPointSize()
{
	return size;
}
