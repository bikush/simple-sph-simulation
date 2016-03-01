
#include "MarchingSmoothSquares.h"
#include "MarchingSquaresFactory.h"
#include "Utility.h"
#include "MappedData.h"
#include <iostream>
#include <gl\glew.h>

using namespace std;

MarchingSmoothSquares::MarchingSmoothSquares( ) : MarchingSquaresBase( )
{
	dataField = NULL;
	maxValue = 0;
}

MarchingSmoothSquares::MarchingSmoothSquares(int w, int h, char max, vec3f p, vec3f s ) :
							MarchingSquaresBase( w, h, p, s )
{
	dataField = new char[ dataWidth * dataHeight ];
	maxValue = max;
	clear();
}

MarchingSmoothSquares::MarchingSmoothSquares( const char* file )
	: MarchingSquaresBase( MappedData( file ) )
{
	MappedData md( file );
	maxValue = (char)md.getData( "smooth", "maxValue" ).getInt();
	dataField = new char[ dataWidth * dataHeight ];
	clear();
}

MarchingSmoothSquares::~MarchingSmoothSquares()
{
	delete [] dataField;
}

void MarchingSmoothSquares::set( int x, int y, float value )
{
	if( x<0 || y<0 || x>=dataWidth || y>=dataHeight) return;
	dataField[x*dataWidth+y] = (char)contain<int>( (int)dataField[x*dataWidth+y]+int(value*maxValue), 0, maxValue );
	//dataField[x*dataWidth+y] = value>1 ? maxValue : value*maxValue;
	dataChanged = true;
}

void MarchingSmoothSquares::inc( int x, int y, char value )
{
	if( x<0 || y<0 || x>=dataWidth || y>=dataHeight) return;
	dataField[x*dataWidth+y] = contain<char>( dataField[x*dataWidth+y]+value, 0, maxValue );
	dataChanged = true;
}

void MarchingSmoothSquares::dec( int x, int y, float value )
{
	char newValue = char(value*maxValue);
	if( x<0 || y<0 || x>=dataWidth || y>=dataHeight) return;
	int index = x*dataWidth+y;
	dataField[index] = newValue > dataField[index] ? 0 : dataField[index]-newValue;
	dataChanged = true;
}

void MarchingSmoothSquares::clear()
{
	for(int i=0; i<dataWidth*dataHeight; i++)
		dataField[i]=0;
	dataChanged = false;
	trianglesCount = 0;
}

void MarchingSmoothSquares::drawGrid(vec3f colorFalse, vec3f colorTrue)
{
	glMatrixMode(GL_MODELVIEW);	
	glPushMatrix();
	glTranslatef(position.x,position.y,position.z);
	glScalef( scale.x, scale.y, scale.z );

	glBegin(GL_POINTS);	
	glColor3f( colorFalse.x, colorFalse.y, colorFalse.z );
	for(int i=0; i<dataWidth; i++)
	{
		for(int j=0; j<dataHeight; j++)
		{
			if(!dataField[i*dataWidth+j])
			{				
				glVertex2f((float)i, (float)j );
			}
		}
	}
	glEnd();

	glBegin(GL_QUADS);	
	float o = 0.2f;
	float fac = 0;
	for(int i=0; i<dataWidth; i++)
	{
		for(int j=0; j<dataHeight; j++)
		{
			if(dataField[i*dataWidth+j])
			{
				fac = ( dataField[i*dataWidth+j]/(float)maxValue );
				glColor3f( fac*colorTrue.x, fac*colorTrue.y, fac*colorTrue.z );
				glVertex2f( i+o, j-o );
				glVertex2f( i+o, j+o );
				glVertex2f( i-o, j+o );
				glVertex2f( i-o, j-o );				
			}
		}
	}
	glEnd();

	glPopMatrix();
}

int MarchingSmoothSquares::generateTrianglesInField( int x, int y, int startIndex )
{
	char ind[4];
	ind[0] = dataField[x*dataWidth + y];
	ind[1] = dataField[x*dataWidth + y+1];
	ind[2] = dataField[(x+1)*dataWidth + y+1];
	ind[3] = dataField[(x+1)*dataWidth + y];
			
	return MarchingSquaresFactory::getInterpolatedSquare( 
					ind, triangles, startIndex,	vec2f( x, y ), maxValue
			);
}