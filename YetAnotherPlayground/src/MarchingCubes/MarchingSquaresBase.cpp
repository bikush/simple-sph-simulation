#include "MarchingSquaresBase.h"
#include "MarchingSquaresFactory.h"
#include <gl\glew.h>

MarchingSquaresBase::MarchingSquaresBase(void)
{
	dataWidth = 0;
	dataHeight = 0;
	dataChanged = false;

	triangles = NULL;
	trianglesCount = 0;
	trianglesSize = 0;
}

MarchingSquaresBase::MarchingSquaresBase(int w, int h, vec3f p, vec3f s)
{
	position = p;	
	scale = s;
		
	dataWidth = w;
	dataHeight = h;
	dataChanged = false;
	
	triangles = new vec2f[ TRIANGLE_COUNT_INCREASE ];
	trianglesCount = 0;
	trianglesSize = TRIANGLE_COUNT_INCREASE;
}

MarchingSquaresBase::MarchingSquaresBase( MappedData& paramFile )
{
	position = paramFile.getData("base","position").getVec3f();	
	scale = paramFile.getData("base","scale").getVec3f();	
		
	dataWidth = paramFile.getData("base","dataWidth").get<int>();
	dataHeight = paramFile.getData("base","dataHeight").get<int>();
	dataChanged = false;
	
	triangles = new vec2f[ TRIANGLE_COUNT_INCREASE ];
	trianglesCount = 0;
	trianglesSize = TRIANGLE_COUNT_INCREASE;
}


MarchingSquaresBase::~MarchingSquaresBase(void)
{
	delete [] triangles;
}

int MarchingSquaresBase::getHeight()
{
	return dataHeight;
}

int MarchingSquaresBase::getWidth()
{
	return dataWidth;
}

/**
 * Outputs the triangles using openGL vertex pointer and array drawing.
 *
 * Param: color of the drawn triangles.
 **/
void MarchingSquaresBase::drawSquares( vec3f color )
{	
	if(dataChanged) generateTriangles();	
	if( trianglesCount == 0 ) return;

	glColor3f( color.x, color.y, color.z );

	glEnableClientState( GL_VERTEX_ARRAY );
	glVertexPointer( 2, GL_FLOAT, 0, triangles );

		GLboolean culling;
		glGetBooleanv( GL_CULL_FACE, &culling );
		if( culling ) glDisable( GL_CULL_FACE );
						
			glMatrixMode(GL_MODELVIEW);	
			glPushMatrix();

				glTranslatef( position.x, position.y, position.z );
				glScalef( scale.x, scale.y, scale.z );
				glDrawArrays( GL_TRIANGLES, 0, trianglesCount );

			glPopMatrix();

		if( culling ) glEnable( GL_CULL_FACE );

	glDisableClientState( GL_VERTEX_ARRAY );
}

/**
 * Traverses the data field and generates new triangles for each cell.
 * Resizes triangles field if there is a posibility of overflow in the next iteration.
 * The resizing is linear.
 **/
void MarchingSquaresBase::generateTriangles()
{
	int start = 0;
	
	for(int i=0; i<dataWidth-1; i++)
	{
		for(int j=0; j<dataHeight-1; j++)
		{
			start += generateTrianglesInField(i,j, start);

			if( start > trianglesSize - MarchingSquaresFactory::MAX_VERTICES )
			{
				int newSize = trianglesSize + TRIANGLE_COUNT_INCREASE;				
				vec2f* newTriangles = new vec2f[ newSize ];

				memcpy( newTriangles, triangles, trianglesSize * sizeof(vec2f) );

				trianglesSize = newSize;
				delete [] triangles;
				triangles = newTriangles;
			}
		}
	}
	trianglesCount = start;

	dataChanged = false;
}

/**
 * Putting a circle consists of setting appropriate fields.
 **/
void MarchingSquaresBase::putCircle( int x, int y, int r )
{
	_putCircleUsingSetter( x, y, r, &MarchingSquaresBase::set );
}

/**
 * Removing a circle consists of decreasing the value of appropriate fields.
 **/
void MarchingSquaresBase::removeCircle( int x, int y, int r )
{
	_putCircleUsingSetter( x, y, r, &MarchingSquaresBase::dec );
}

/**
 * Traverses all data fields that could be in the circle and calls the setter function.
 **/
void MarchingSquaresBase::_putCircleUsingSetter( int x, int y, int r, void (MarchingSquaresBase::*setter)(int,int,float) )
{
	float distance; // distance of a point from circle center
	for(int i=x-r; i<=x+r; i++)
	{
		for(int j=y-r; j<=y+r; j++)
		{			
			distance = (float)r - sqrtf( powf( (float)(i-x), 2.f ) + powf((float)(j-y), 2.f ) );
			if( distance > 0 )
			{				
				(this->*setter)( i, j, distance>1 ? 1:distance );
			}
		}
	}
}

/**
 * Putting a circle consists of setting appropriate fields. More precise with floats.
 **/
void MarchingSquaresBase::putCircle( float x, float y, float r, bool dummy )
{
	float distance; // distance of a point from circle center
	for(float i=x-r; i<=x+r; i++)
	{
		for(float j=y-r; j<=y+r; j++)
		{			
			distance = r - sqrtf( powf( i-x, 2 ) + powf( j-y, 2 ) );
			if( distance > 0 )
			{				
				set( (int) i, (int) j, distance>1 ? 1:distance );
			}
		}
	}
}