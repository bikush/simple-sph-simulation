
#include "MarchingCubesShaded.h"
#include "MappedData.h"
#include "ShaderProgram.h"
#include "MarchingCubesFactory.h"
#include "Camera.h"
#include <glm\geometric.hpp>
#include <glm\common.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>

using namespace std;

MarchingCubesShaded::MarchingCubesShaded( const char* filePath )
{
	MappedData paramFile( filePath );

	vPosition = paramFile.getData("base","position").getVec3();	
	vSpan = paramFile.getData("base","span").getVec3();	
		
	dataWidth = paramFile.getData("base","dataWidth").get<int>();
	dataHeight = paramFile.getData("base","dataHeight").get<int>();
	dataDepth = paramFile.getData("base","dataDepth").get<int>();
	dataWidth = dataWidth<DATA_MIN ? DATA_MIN : dataWidth;
	dataHeight = dataHeight<DATA_MIN ? DATA_MIN : dataHeight;
	dataDepth = dataDepth<DATA_MIN ? DATA_MIN : dataDepth;
	
	dataMax = paramFile.getData("base","maxValue").get<float>();
	treshold = paramFile.getData("base","treshold").get<float>( 0.5f );

	string vertex = paramFile.getData("shaders", "v").getStringData();
	string geometry = paramFile.getData("shaders", "g").getStringData();
	string fragment = paramFile.getData("shaders", "f").getStringData();

	mcShader = ShaderProgram::CreateShader( vertex, geometry, fragment );

	GLuint programHandle = mcShader->getProgramID();

	initDataField( );	
		
	initGridBuffer( );
		
	mcShader->turnOn();
		mcShader->setUniformI( "Data", 0 );
		mcShader->setUniformI( "TriTable", 1 );
		
		mcShader->setUniformV3( "VerticeOffsets[0]", 0.0f,			0.0f,		0.0f		); 
		mcShader->setUniformV3( "VerticeOffsets[1]", gridStep.x,	0.0f,		0.0f		); 
		mcShader->setUniformV3( "VerticeOffsets[2]", gridStep.x,	gridStep.y, 0.0f		); 
		mcShader->setUniformV3( "VerticeOffsets[3]", 0.0f,			gridStep.y, 0.0f		); 
		mcShader->setUniformV3( "VerticeOffsets[4]", 0.0f,			0.0f,		gridStep.z	); 
		mcShader->setUniformV3( "VerticeOffsets[5]", gridStep.x,	0.0f,		gridStep.z	); 
		mcShader->setUniformV3( "VerticeOffsets[6]", gridStep.x,	gridStep.y, gridStep.z	); 
		mcShader->setUniformV3( "VerticeOffsets[7]", 0.0f,			gridStep.y, gridStep.z	);

		mcShader->setUniformV3("Color", 0.0f, 0.3f, 1.0f);
		mcShader->setUniformV3("Diffuse", 0.4f, 0.9f, 0.0f);
		mcShader->setUniformV3("Specular", 1.0f, 1.0f, 1.0f);
		mcShader->setUniformV3("Ambient", 0.3f, 0.8f, 0.6f);
		mcShader->setUniformV3("LightPosition", 20.f, -15.0f, 5.0f);

		mcShader->setUniformV3( "DataStep", 1.0f/dataWidth, 1.0f/dataHeight, 1.0f/dataDepth );
	mcShader->turnOff();

	clear();
}

MarchingCubesShaded::~MarchingCubesShaded( )
{
	delete [] dataField;
	// delete buffer and vao
	glDeleteBuffers(1,&gridHandle);
	glDeleteVertexArrays(1,&gridVao);
}

// Create a 3D grid of points in the span od 0,0,0 - 1,1,1
// This will be bound to the VAO and sent to the shader as vertices
// Actual data that generates the marching cube mesh is sent as a 3D texture
void MarchingCubesShaded::initGridBuffer( )
{
	gridStep = glm::vec3(1,1,1) / (glm::vec3(dataWidth, dataHeight, dataDepth)-glm::vec3(1,1,1));
	gridElementCount = (dataWidth + 1)*(dataHeight + 1)*(dataDepth + 1);

	int gridTotalSize =  gridElementCount*3;
	float* grid = new float[gridTotalSize];
	int index = 0;
	bool done = false;
	for( float x = -gridStep.x; x <= 1.0f && !done; x+=gridStep.x )
	{
		for( float y = -gridStep.y; y <= 1.0f && !done; y+=gridStep.y )
		{
			for( float z = -gridStep.z; z <= 1.0f && !done; z+=gridStep.z )
			{
				grid[ index   ] = x;
				grid[ index+1 ] = y;
				grid[ index+2 ] = z;
				index += 3;	
				done = index >= gridTotalSize;
			}
		}
	}
	
	glGenBuffers(1, &gridHandle);
	glBindBuffer( GL_ARRAY_BUFFER, gridHandle );
	glBufferData( GL_ARRAY_BUFFER, gridTotalSize * sizeof(float), grid, GL_STATIC_DRAW );
	delete[] grid;
	
	glGenVertexArrays( 1, &gridVao );
	glBindVertexArray( gridVao );
	glEnableVertexAttribArray(0);
	glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 0, (GLubyte *)NULL );

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDisableVertexAttribArray(0);
}

void MarchingCubesShaded::initDataField( )
{
	// MC DATA
	glGenTextures(1, &dataTexID);

	glActiveTexture(GL_TEXTURE0);
	glEnable(GL_TEXTURE_3D);
	glBindTexture(GL_TEXTURE_3D, dataTexID);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glDisable(GL_TEXTURE_3D);

	dataSize = dataWidth * dataHeight * dataDepth;
	deltaSpan = vSpan / glm::vec3( dataWidth, dataHeight, dataDepth );
	dataField = new float[ dataSize ]();
	dataChanged = true;
	// data is updated when changed
//	glTexImage3D( GL_TEXTURE_3D, 0, GL_ALPHA32F_ARB, dataWidth, dataHeight, dataDepth, 0, GL_ALPHA, GL_FLOAT, dataField);
	
}

void MarchingCubesShaded::setUnchecked( int x, int y, int z, float value )
{
	float &data = dataField[ x*dataHeight*dataDepth + y*dataDepth + z ];
	data = contain(value+data, 0.0f, dataMax);
	dataChanged = true;
}

void MarchingCubesShaded::set( int x, int y, int z, float value )
{
	if( x<1 || y<1  || z<1 || x>=dataWidth-1 || y>=dataHeight-1 || z>=dataDepth-1 ) return;	
	float &data = dataField[ x*dataHeight*dataDepth + y*dataDepth + z ];
	data = contain(value+data, 0.0f, dataMax);
	dataChanged = true;
}

void MarchingCubesShaded::set( int position, float value )
{
	if( position<0 || position>dataSize ) return;	
	dataField[ position ] = contain(value, 0.0f, dataMax);
	dataChanged = true;
}

void MarchingCubesShaded::clear()
{
	memset( dataField, 0, dataSize*sizeof(float) );
	dataChanged = true;
}

glm::vec3 MarchingCubesShaded::getScale()
{
	return vSpan;
}

glm::vec3 MarchingCubesShaded::getPosition()
{
	return vPosition;
}

float MarchingCubesShaded::getTreshold()
{
	return this->treshold;
}

void MarchingCubesShaded::setTreshold( float t )
{
	this->treshold = t;
}

void MarchingCubesShaded::putSphere( float x, float y, float z, float r )
{
	float value;
	glm::vec3 start( x-r, y-r, z-r );
	start -= vPosition;
	start /= deltaSpan;

	glm::vec3 end( x+r, y+r, z+r );
	end -= vPosition;
	end /= deltaSpan;

	glm::vec3 center = (glm::vec3( x,y,z ) - vPosition) / deltaSpan;
	r /= glm::length( deltaSpan );

	glm::ivec3 iStart( start.x, start.y, start.z );
	glm::ivec3 iEnd( end.x+1, end.y+1, end.z+1 );
	for(int i= iStart.x; i<iEnd.x; i++)
	{
		for(int j= iStart.y; j<iEnd.y; j++)
		{
			for(int k= iStart.z; k<iEnd.z; k++)
			{
				value = r-glm::length(center-glm::vec3(i,j,k))+1;
				if (value > 0)
				{
					set(i, j, k, value);
				}
			}
		}
	}
}

void MarchingCubesShaded::draw(const Camera& camera)
{
	glm::mat4 mvp = camera.getViewProjection() * transform.getTransformMatrix();
	glm::vec3 eye = camera.getPosition();
		
	glDisable( GL_CULL_FACE );
	mcShader->turnOn();
		mcShader->setUniformF( "Treshold", this->treshold );		
		mcShader->setUniformV3( "Eye", eye.x, eye.y, eye.z );
		mcShader->setUniformM4( "MVP", glm::value_ptr(mvp) );
	
		MarchingCubesFactory::setTexture( GL_TEXTURE1 );
	
		glEnable( GL_TEXTURE_3D );
		glActiveTexture( GL_TEXTURE0 );
		glBindTexture( GL_TEXTURE_3D, dataTexID );
		if (dataChanged)
		{
			glTexImage3D(GL_TEXTURE_3D, 0, GL_ALPHA32F_ARB, dataWidth, dataHeight, dataDepth, 0, GL_ALPHA, GL_FLOAT, dataField);
			dataChanged = false;
		}

		// !
		GLboolean blendEnabled = glIsEnabled( GL_BLEND );
		glEnable( GL_BLEND );
		glBlendFunc(GL_ONE, GL_ONE);
		glDepthMask(GL_FALSE);
			glBindVertexArray(gridVao);
			glDrawArrays(GL_POINTS, 0, gridElementCount );
			glBindVertexArray(0);
		glDepthMask(GL_TRUE);
		if (!blendEnabled) { 
			glDisable(GL_BLEND); 
		}

		glDisable( GL_TEXTURE_3D);

		//glEnable(GL_TEXTURE_2D);
		//glActiveTexture(GL_TEXTURE1);
		//glBindTexture(GL_TEXTURE_2D, 0);
	mcShader->turnOff();
	glEnable( GL_CULL_FACE );
}