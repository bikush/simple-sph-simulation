
#include "MarchingCubesBasic.h"
#include "MarchingCubesFactory.h"
#include "Utility.h"
#include "MappedData.h"
#include "ShaderProgram.h"
#include <glm\common.hpp>
#include <glm\geometric.hpp>

MarchingCubesBasic::MarchingCubesBasic( )
{
	dataField = NULL;
	dataWidth = 0;
	dataHeight = 0;
	dataDepth = 0;
	position = vec3f(0,0,0);
	span = vec3f(1,1,1);
	dSpan = vec3f(0,0,0);
	triangles = NULL;
	normals = 0;
	trianglesCount = 0;
	trianglesSize = 0;
}

MarchingCubesBasic::MarchingCubesBasic( int w, int h, int d, float treshold, float dataMax, vec3f position, vec3f span ) :
	position( position ), span( span ), dataMax( dataMax ), treshold( treshold )
{
	dataWidth = w<DATA_CHANGE_DIV ? DATA_CHANGE_DIV : w;
	dataHeight = h<DATA_CHANGE_DIV ? DATA_CHANGE_DIV : h;
	dataDepth = d<DATA_CHANGE_DIV ? DATA_CHANGE_DIV : d;
	dataSize = dataWidth * dataHeight * dataDepth;
	dSpan = span / vec3f( dataWidth, dataHeight, dataDepth );

	dataField = new float[ dataSize ];
				
	trianglesCount = 0;
	trianglesSize = TRIANGLE_COUNT_INCREASE;
	triangles = new vec3f[ trianglesSize ];
	normals = new vec3f[trianglesSize];

	clear();
}

MarchingCubesBasic::MarchingCubesBasic( const char* filePath )
{
	MappedData paramFile( filePath );

	position = paramFile.getData("base","position").getVec3f();	
	span = paramFile.getData("base","span").getVec3f();	
		
	dataWidth = paramFile.getData("base","dataWidth").get<int>();
	dataHeight = paramFile.getData("base","dataHeight").get<int>();
	dataDepth = paramFile.getData("base","dataDepth").get<int>();
	dataWidth = dataWidth<DATA_CHANGE_DIV ? DATA_CHANGE_DIV : dataWidth;
	dataHeight = dataHeight<DATA_CHANGE_DIV ? DATA_CHANGE_DIV : dataHeight;
	dataDepth = dataDepth<DATA_CHANGE_DIV ? DATA_CHANGE_DIV : dataDepth;
	dataSize = dataWidth * dataHeight * dataDepth;
	dSpan = span / vec3f( dataWidth, dataHeight, dataDepth );
	dataField = new float[ dataSize ];

	dataMax = paramFile.getData("base","maxValue").get<float>();
	treshold = paramFile.getData("base","treshold").get<float>();

	trianglesCount = 0;
	trianglesSize = TRIANGLE_COUNT_INCREASE;
	triangles = new vec3f[ trianglesSize ];
	normals = new vec3f[trianglesSize];

	clear();
}

MarchingCubesBasic::~MarchingCubesBasic()
{
	delete [] dataField;
	delete [] triangles;
	delete [] normals;
}

void MarchingCubesBasic::set( int x, int y, int z, float value )
{
	if( x<1 || y<1  || z<1 || x>=dataWidth-1 || y>=dataHeight-1 || z>=dataDepth-1 ) return;	
	float &data = dataField[ x*dataHeight*dataDepth + y*dataDepth + z ];
	data = contain(value+data, 0.0f, dataMax);
}

void MarchingCubesBasic::set( int position, float value )
{
	if( position<0 || position>this->dataSize ) return;	
	dataField[ position ] = contain(value, 0.0f, dataMax);
}

void MarchingCubesBasic::setUnchecked( int x, int y, int z, float value )
{
	float &data = dataField[ x*dataHeight*dataDepth + y*dataDepth + z ];
	data = contain(value+data, 0.0f, dataMax);
}

void MarchingCubesBasic::clear()
{
	for(int i=0; i<dataWidth*dataHeight*dataDepth; i++)
	{
		dataField[i] = 0;
	}
	trianglesCount = 0;
}

void MarchingCubesBasic::drawGrid( vec3f colorFalse, vec3f colorTrue )
{	
	glMatrixMode(GL_MODELVIEW);	
	glPushMatrix();
	glTranslatef(position.x,position.y,position.z);
	//glScalef( span.x, span.y, span.z );
		
	glBegin(GL_POINTS);	
	float value;
	vec3f color;
	float x=0;
	float y=0;
	float z=0;
	for(int i=0; i<dataWidth; i++)
	{
		y = 0;
		for(int j=0; j<dataHeight; j++)
		{
			z = 0;
			for(int k=0; k<dataDepth; k++)
			{		
				value = dataField[ i*dataHeight*dataDepth + j*dataDepth	+ k ];
				if( value > treshold && value < dataMax/2.0 )
				{
					color = colorTrue * ((value-treshold) / dataMax);
					glColor3f( color.x, color.y, color.z );
					glVertex3f( x, y, z );
				}
				z+=dSpan.z;
			}
			y+=dSpan.y;
		}
		x+=dSpan.x;
	}
	glEnd();

	glBegin(GL_POINTS);	
	glColor3f( colorFalse.x, colorFalse.y, colorFalse.z );
	x = 0;
	for(int i=0; i<dataWidth; i++)
	{
		y = 0;
		for(int j=0; j<dataHeight; j++)
		{
			z = 0;
			for(int k=0; k<dataDepth; k++)
			{
				if( dataField[ i*dataHeight*dataDepth + j*dataDepth	+ k ] < treshold )
				{
					glVertex3f( x, y, z );		
				}
				z+=dSpan.z;
			}
			y+=dSpan.y;
		}
		x+=dSpan.x;
	}
	glEnd();
	glPopMatrix();
}

void MarchingCubesBasic::generateTriangles()
{
	trianglesCount = 0;	// reset the triangle buffer

	int HD = dataHeight*dataDepth;
	int iHD;	// i*dataHeight*dataDepth
	int iHDjD;  // i*dataHeight*dataDepth + j*dataDepth
	int index0, index1, index2, index3;	// helping indices

	float cube[8];	
	for(int i=0; i<dataWidth-1; i++)
	{
		iHD = i*HD;
		for(int j=0; j<dataHeight-1; j++)
		{
			iHDjD = iHD + j*dataDepth;
			for(int k=0; k<dataDepth-1; k++)
			{
				index0 = iHDjD  + k;			// i*dataHeight*dataDepth + j*dataDepth + k
				index1 = index0 + dataDepth;	// i*dataHeight*dataDepth + (j+1)*dataDepth + k
				index2 = index1 + HD;			// (i+1)*dataHeight*dataDepth + (j+1)*dataDepth + k
				index3 = index0 + HD;			// (i+1)*dataHeight*dataDepth + j*dataDepth + k

				cube[0] = dataField[index0];
				cube[1] = dataField[index1];
				cube[2] = dataField[index2];
				cube[3] = dataField[index3];
				cube[4] = dataField[index0 + 1 ];
				cube[5] = dataField[index1 + 1 ];
				cube[6] = dataField[index2 + 1 ];
				cube[7] = dataField[index3 + 1 ];

				trianglesCount += MarchingCubesFactory::getFloatInterpolatedCube( cube, triangles, normals, trianglesCount, vec3f(i,j,k), dataMax, treshold);
			
				if( trianglesCount > trianglesSize - MarchingCubesFactory::MAX_VERTICES )
				{
					int newSize = trianglesSize + TRIANGLE_COUNT_INCREASE;				
					vec3f* newTriangles = new vec3f[ newSize ];
					vec3f* newNormals = new vec3f[ newSize ];

					memcpy( newTriangles, triangles, trianglesSize * sizeof(vec3f) );
					memcpy( newNormals, normals, trianglesSize * sizeof(vec3f) );

					trianglesSize = newSize;
					delete [] triangles;
					delete [] normals;
					triangles = newTriangles;
					normals = newNormals;
				}
			}
		}
	}
	dataChanged = false;
}


void MarchingCubesBasic::drawColoredCubes( vec3f color )
{
	glDisable( GL_LIGHTING );
	glColor3f( color.x, color.y, color.z );
	
	drawTriangleBuffer();
}

void MarchingCubesBasic::drawLightedCubes( GLfloat material[4] )
{
	glEnable(GL_LIGHTING);
	glMaterialfv( GL_FRONT, GL_AMBIENT_AND_DIFFUSE, material );
	glMaterialfv( GL_FRONT, GL_SPECULAR, material );
	
	drawTriangleBuffer();
	glDisable( GL_LIGHTING );
}

void MarchingCubesBasic::drawTriangleBuffer()
{
	if(dataChanged) generateTriangles();		
	if( trianglesCount == 0 ) return;

	glEnableClientState( GL_VERTEX_ARRAY );
	glEnableClientState( GL_NORMAL_ARRAY );
		
	glVertexPointer( 3, GL_FLOAT, 0, triangles );
	glNormalPointer( GL_FLOAT, 0, normals );
					
	glMatrixMode(GL_MODELVIEW);	
	glPushMatrix();

		glTranslatef( position.x, position.y, position.z );
		glScalef( dSpan.x, dSpan.y, dSpan.z );
		glDrawArrays( GL_TRIANGLES, 0, trianglesCount );

	glPopMatrix();

	glDisableClientState( GL_VERTEX_ARRAY );
	glDisableClientState( GL_NORMAL_ARRAY );

}

void MarchingCubesBasic::putSphere( float x, float y, float z, float r )
{
	float value;
	vec3f start = vec3f( x-r, y-r, z-r );
	start = glm::clamp( start, position, position+span );
	start -= position;
	start /= dSpan;

	vec3f end = vec3f( x+r, y+r, z+r );
	start = glm::clamp( end, position, position+span );
	end -= position;
	end /= dSpan;

	vec3f center = (vec3f( x,y,z ) - position) / dSpan;
	r /= glm::length( dSpan );

	for(int i= (int)start.x; i<(int)end.x; i++)
	{
		for(int j= (int)start.y; j<(int)end.y; j++)
		{
			for(int k= (int)start.z; k<(int)end.z; k++)
			{
				value = r-glm::length(center-vec3f(i,j,k))+1;
				if( value > 0 )
					set( i, j, k, value );
				
			}
		}
	}

	dataChanged = true;
}


vec3f MarchingCubesBasic::getScale()
{
	return span;
}

vec3f MarchingCubesBasic::getPosition()
{
	return position;
}