
#include "MarchingSquares.h"
#include "MarchingSquaresFactory.h"
#include <iostream>
#include <gl\glew.h>

using namespace std;

MarchingSquares::MarchingSquares( ) : MarchingSquaresBase( )
{
	dataField = NULL;
}

MarchingSquares::MarchingSquares(int w, int h, glm::vec3 p, glm::vec3 s ) :
					MarchingSquaresBase( w, h, p, s )
{
	dataField = new bool[ dataWidth * dataHeight ];
	clear();
}

MarchingSquares::~MarchingSquares()
{
	delete [] dataField;
}

void MarchingSquares::set( int x, int y, float value )
{
	if( x<0 || y<0 || x>=dataWidth || y>=dataHeight) return;
	dataField[x*dataWidth+y] = value!=0 ;
	dataChanged = true;
}

void MarchingSquares::dec( int x, int y, float value )
{
	if( x<0 || y<0 || x>=dataWidth || y>=dataHeight || value == 0) return;
	dataField[x*dataWidth+y] = 0;
	dataChanged = true;
}

void MarchingSquares::flip( int x, int y )
{
	if( x<0 || y<0 || x>=dataWidth || y>=dataHeight) return;
	dataField[x*dataWidth+y] = !dataField[x*dataWidth+y];
	dataChanged = true;
}

void MarchingSquares::clear()
{
	for(int i=0; i<dataWidth*dataHeight; i++)
		dataField[i]=false;
	dataChanged = false;
	trianglesCount = 0;
}

void MarchingSquares::drawGrid(glm::vec3 colorFalse, glm::vec3 colorTrue)
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
				glVertex2f( (float)i, (float)j );
			}
		}
	}
	glEnd();

	glBegin(GL_QUADS);
	glColor3f( colorTrue.x, colorTrue.y, colorTrue.z );
	float o = 0.2f;
	for(int i=0; i<dataWidth; i++)
	{
		for(int j=0; j<dataHeight; j++)
		{
			if(dataField[i*dataWidth+j])
			{
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

int MarchingSquares::generateTrianglesInField( int x, int y, int startIndex )
{
	return MarchingSquaresFactory::getSquare( 
				dataField[x*dataWidth + y],
				dataField[x*dataWidth + y+1],
				dataField[(x+1)*dataWidth + y+1],
				dataField[(x+1)*dataWidth + y], 
				triangles, 
				startIndex,
				glm::vec2( x, y ));
}

void MarchingSquares::putRectangle( int x1, int y1, int x2, int y2, bool value )
{
	int tmp;
	if( x1 > x2 ){
		tmp = x1;
		x1 = x2;
		x2 = tmp;
	}
	if( y1 > y2 ){
		tmp = y1;
		y1 = y2;
		y2 = tmp;
	}

	for(int i=x1; i<=x2; i++)
	{
		for(int j=y1; j<=y2; j++)
		{
			set( i, j, value );
		}
	}
}