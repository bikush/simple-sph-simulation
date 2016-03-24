
#include "MarchingCubes.h"
#include "MarchingCubesFactory.h"
#include "Utility.h"
#include "MappedData.h"
#include "DataLine.h"
#include "ShaderProgram.h"

MarchingCubes::MarchingCubes( )
{
	dataField = NULL;
	dataWidth = 0;
	dataHeight = 0;
	dataDepth = 0;
	dataChanged = new bool[ DATA_CHANGE_SIZE ];
	dataChangedGlobal = false;
	position = vec3f(0,0,0);
	scale = vec3f(1,1,1);
	triangles = NULL;
	normals = 0;
	trianglesCount = 0;
	trianglesSize = 0;
	useInterpolated = true;

	pointGrid = 0;
}

MarchingCubes::MarchingCubes( int w, int h, int d, char maxValue, vec3f pos, vec3f scl )
{
	dataWidth = w<DATA_CHANGE_DIV ? DATA_CHANGE_DIV : w;
	dataHeight = h<DATA_CHANGE_DIV ? DATA_CHANGE_DIV : h;
	dataDepth = d<DATA_CHANGE_DIV ? DATA_CHANGE_DIV : d;
	dataSize = dataWidth * dataHeight * dataDepth;

	dataChanged = new bool[ DATA_CHANGE_SIZE ];
	dataChangedWidth = (dataWidth/DATA_CHANGE_DIV);
	dataChangedHeight = (dataHeight/DATA_CHANGE_DIV);
	dataChangedDepth = (dataDepth/DATA_CHANGE_DIV);
	dataChangedGlobal = false;
	
	dataMax = maxValue;
	treshold = 0;
	dataField = new char[ dataSize ];
	useInterpolated = true;

	position = pos;
	scale = scl;

	trianglesCount = 0;
	trianglesSize = TRIANGLE_COUNT_INCREASE;
	triangles = new vec3f[ trianglesSize ];
	normals = new vec3f[trianglesSize];

	pointGrid = 0;

	clear();
}

MarchingCubes::MarchingCubes( const char* filePath )
{
	MappedData paramFile( filePath );

	position = paramFile.getData("base","position").getVec3f();	
	scale = paramFile.getData("base","scale").getVec3f();	
		
	dataWidth = paramFile.getData("base","dataWidth").get<int>();
	dataHeight = paramFile.getData("base","dataHeight").get<int>();
	dataDepth = paramFile.getData("base","dataDepth").get<int>();
	dataWidth = dataWidth<DATA_CHANGE_DIV ? DATA_CHANGE_DIV : dataWidth;
	dataHeight = dataHeight<DATA_CHANGE_DIV ? DATA_CHANGE_DIV : dataHeight;
	dataDepth = dataDepth<DATA_CHANGE_DIV ? DATA_CHANGE_DIV : dataDepth;
	dataSize = dataWidth * dataHeight * dataDepth;
	dataField = new char[ dataSize ];

	dataChanged = new bool[ DATA_CHANGE_SIZE ];
	dataChangedWidth = (dataWidth/DATA_CHANGE_DIV);
	dataChangedHeight = (dataHeight/DATA_CHANGE_DIV);
	dataChangedDepth = (dataDepth/DATA_CHANGE_DIV);
	dataChangedGlobal = false;
	
	dataMax = paramFile.getData("base","maxValue").get<int>();
	treshold = 0;
	useInterpolated = true;

	trianglesCount = 0;
	trianglesSize = TRIANGLE_COUNT_INCREASE;
	triangles = new vec3f[ trianglesSize ];
	normals = new vec3f[trianglesSize];

	pointGrid = 0;

	clear();
}

MarchingCubes::~MarchingCubes()
{
	delete [] dataField;
	delete [] dataChanged;
	delete [] triangles;
	delete [] normals;
	if( pointGrid ) delete [] pointGrid;
}

void MarchingCubes::setDataChanged( int x, int y, int z, bool value )
{
	int dx = x / dataChangedWidth;
	int dy = y / dataChangedHeight;
	int dz = z / dataChangedDepth;
	dataChanged[ dx*DATA_CHANGE_DIV*DATA_CHANGE_DIV + dy*DATA_CHANGE_DIV + dz ] = value;
	dataChangedGlobal = true;
}

bool MarchingCubes::getDataChanged( int x, int y, int z )
{
	int dx = x / dataChangedWidth;
	int dy = y / dataChangedHeight;
	int dz = z / dataChangedDepth;	
	return dataChanged[ dx*DATA_CHANGE_DIV*DATA_CHANGE_DIV + dy*DATA_CHANGE_DIV + dz ];
}

void MarchingCubes::setAllDataChanged( bool value )
{
	for(int i=0; i<DATA_CHANGE_SIZE; i++) dataChanged[i] = value;
	dataChangedGlobal = value;
}

void MarchingCubes::set( int x, int y, int z, float value )
{
	if( x<1 || y<1  || z<1 || x>=dataWidth-1 || y>=dataHeight-1 || z>=dataDepth-1 ) return;	
	dataField[ x*dataHeight*dataDepth + y*dataDepth + z ] = char(contain(value, 0.0f, 1.0f) * (float)dataMax);
	setDataChanged( x,y,z );
}
/*
void MarchingCubes::set( int x, int y, int z, char value )
{
	if( x<0 || y<0  || z<0 || x>=dataWidth || y>=dataHeight || z>=dataDepth ) return;
	value = value < 0 ? 0 : (value>dataMax ? dataMax : value);
	dataField[ x*dataHeight*dataDepth + y*dataDepth + z ] = value;
	dataChanged = true;
}*/

void MarchingCubes::flip( int x, int y, int z )
{
	if( x<0 || y<0  || z<0 || x>=dataWidth || y>=dataHeight || z>=dataDepth) return;	
	int index = x*dataHeight*dataDepth + y*dataDepth + z ;
	dataField[ index ] = dataMax - dataField[ index ];
	setDataChanged( x,y,z );
}

void MarchingCubes::clear()
{
	for(int i=0; i<dataWidth*dataHeight*dataDepth; i++)
	{
		dataField[i] = 0;
	}
	setAllDataChanged( false );
	trianglesCount = 0;
}

void MarchingCubes::drawGrid( vec3f colorFalse, vec3f colorTrue )
{	
	glMatrixMode(GL_MODELVIEW);	
	glPushMatrix();
	glTranslatef(position.x,position.y,position.z);
	glScalef( scale.x, scale.y, scale.z );
		
	glBegin(GL_POINTS);
	glColor3f( colorTrue.x, colorTrue.y, colorTrue.z );
	for(int i=0; i<dataWidth; i++)
	{
		for(int j=0; j<dataHeight; j++)
		{
			for(int k=0; k<dataDepth; k++)
			{				
				if( dataField[ i*dataHeight*dataDepth + j*dataDepth	+ k ] )
				{
					glVertex3f((float)i, (float)j, (float)k );
				}
			}
		}
	}
	glEnd();

	glBegin(GL_POINTS);	
	glColor3f( colorFalse.x, colorFalse.y, colorFalse.z );
	for(int i=0; i<dataWidth; i++)
	{
		for(int j=0; j<dataHeight; j++)
		{
			for(int k=0; k<dataDepth; k++)
			{
				if( !dataField[ i*dataHeight*dataDepth + j*dataDepth	+ k ] )
				{
					if( getDataChanged( i, j, k ) )
					{						
						glVertex3f((float)i, (float)j, (float)k );
					}
				}
			}
		}
	}
	glEnd();
	glPopMatrix();
}

void MarchingCubes::generateTriangles( int x, int countX, int y, int countY, int z, int countZ )
{
	int HD = dataHeight*dataDepth;
	int iHD;	// i*dataHeight*dataDepth
	int iHDjD;  // i*dataHeight*dataDepth + j*dataDepth
	int index0, index1, index2, index3;	// helping indices

	char cube[8];	
	int cubeIndex;
	// x-1<0 ? 0 : x-1 <- causes slowdon, without it doesnt draw properly
	for(int i=x-1<0?0:x-1; i<dataWidth-1 && i<x+countX ; i++)
	{
		iHD = i*HD;
		for(int j=y-1<0?0:y-1; j<dataHeight-1 && j<y+countY ; j++)
		{
			iHDjD = iHD + j*dataDepth;
			for(int k=z-1<0?0:z-1; k<dataDepth-1 && k<z+countZ ; k++)
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

				if(useInterpolated)
				{
					trianglesCount += MarchingCubesFactory::getInterpolatedCube( cube, dataMax, triangles, normals, trianglesCount, vec3f(i,j,k), treshold);
				}else
				{
					cubeIndex = MarchingCubesFactory::getCubeIndex( cube );				
					trianglesCount += MarchingCubesFactory::getCube( cubeIndex, triangles, normals, trianglesCount, vec3f(i,j,k));
				}

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
	setDataChanged( x, y, z, false );
}


void MarchingCubes::generateTriangles()
{
	int HD = dataHeight*dataDepth;
	int iHD;	// i*dataHeight*dataDepth
	int iHDjD;  // i*dataHeight*dataDepth + j*dataDepth
	int index0, index1, index2, index3;	// helping indices

	char cube[8];	
	int cubeIndex;
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

				if(useInterpolated)
				{
					trianglesCount += MarchingCubesFactory::getInterpolatedCube( cube, dataMax, triangles, normals, trianglesCount, vec3f(i,j,k), treshold);
				}else
				{
					cubeIndex = MarchingCubesFactory::getCubeIndex( cube );				
					trianglesCount += MarchingCubesFactory::getCube( cubeIndex, triangles, normals, trianglesCount, vec3f(i,j,k));
				}

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
	setAllDataChanged( false );
}


void MarchingCubes::drawColoredCubes( vec3f color )
{
	glDisable( GL_LIGHTING );
	glColor3f( color.x, color.y, color.z );
	
	drawTriangleBuffer();
}

void MarchingCubes::drawLightedCubes( GLfloat material[4] )
{
	glEnable(GL_LIGHTING);
	glMaterialfv( GL_FRONT, GL_AMBIENT_AND_DIFFUSE, material );
	glMaterialfv( GL_FRONT, GL_SPECULAR, material );
	
	drawTriangleBuffer();
	glDisable( GL_LIGHTING );
}

void MarchingCubes::drawTriangleBuffer()
{
	//if(dataChanged) generateTriangles();	
	if(dataChangedGlobal)
	{
		trianglesCount = 0;
		for(int i=0; i<DATA_CHANGE_DIV; i++)
		{
			for(int j=0; j<DATA_CHANGE_DIV; j++)
			{
				for(int k=0; k<DATA_CHANGE_DIV; k++)
				{
					if( dataChanged[ i*DATA_CHANGE_DIV*DATA_CHANGE_DIV + j*DATA_CHANGE_DIV + k ] )
					{

						generateTriangles( 
							i*dataChangedWidth, dataChangedWidth, 
							j*dataChangedHeight, dataChangedHeight,
							k*dataChangedDepth, dataChangedDepth );
					}
				}
			}
		}
		dataChangedGlobal = false;
	}
	if( trianglesCount == 0 ) return;

	glEnableClientState( GL_VERTEX_ARRAY );
	glEnableClientState( GL_NORMAL_ARRAY );

	
	glVertexPointer( 3, GL_FLOAT, 0, triangles );
	glNormalPointer( GL_FLOAT, 0, normals );

	GLboolean culling;
	glGetBooleanv( GL_CULL_FACE, &culling );
	if( culling ) glDisable( GL_CULL_FACE );/**/
						
	glMatrixMode(GL_MODELVIEW);	
	glPushMatrix();

		glTranslatef( position.x, position.y, position.z );
		glScalef( scale.x, scale.y, scale.z );
		glDrawArrays( GL_TRIANGLES, 0, trianglesCount );

	glPopMatrix();

	if( culling ) glEnable( GL_CULL_FACE );/**/

	glDisableClientState( GL_VERTEX_ARRAY );
	glDisableClientState( GL_NORMAL_ARRAY );

}

void MarchingCubes::putSphere( float x, float y, float z, float r )
{
	float value;
	for(int i=(int)(x-r); i<(int)(x+r); i++)
	{
		for(int j= (int)(y-r); j<(int)(y+r); j++)
		{
			for(int k= (int)(z-r); k<(int)(z+r); k++)
			{
				value = r-sqrtf(powf( x-i, 2 ) + powf( y-j, 2 ) + powf( z-k, 2 ));
			/*	if( value > 0 )
					set( i, j, k, 1.0 );
				else/**/ 
				if( value > -1 )
					set( i, j, k, 1+value );
				
			}
		}
	}
}

vec3f MarchingCubes::getDataDimensions()
{
	return vec3f( dataWidth, dataHeight, dataDepth );
}

char* MarchingCubes::getData()
{
	return dataField;
}

int MarchingCubes::increaseTreshold()
{
	treshold = (treshold+1)%dataMax;
	return treshold;
}

/****************
 * SHADER STUFF *
 ****************/

void MarchingCubes::drawWithShader( ShaderProgram* shader )
{
	/*shader->turnOn();
	setTexture(GL);
	glBindBuffer(GL_ARRAY_BUFFER, pointGridBuffer);

	glMatrixMode(GL_MODELVIEW);	
	glPushMatrix();
		glTranslatef( position.x, position.y, position.z );
		glScalef( scale.x, scale.y, scale.z );

		glEnableClientState(GL_VERTEX_ARRAY);
			glVertexPointer(3, GL_FLOAT, 0,  NULL);
			glDrawArrays(GL_POINTS, 0, dataSize);
		glDisableClientState(GL_VERTEX_ARRAY);

	glPopMatrix();

	glBindBuffer(GL_ARRAY_BUFFER_ARB, 0);
	shader->turnOff();*/
}

void MarchingCubes::initPointGrid()
{
	pointGrid = new vec3f[ dataSize ];
	int index = 0;
	for(float x = -1; x<1.0f; x += 2.0f/dataWidth)
	{
		for(float y = -1; y<1.0f; y += 2.0f/dataWidth)
		{
			for(float z = -1; z<1.0f; z += 2.0f/dataWidth)
			{
				pointGrid[ index ] = vec3f( x, y, z );
				index++;
			}
		}
	}
	glGenBuffers(1, &pointGridBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, pointGridBuffer);
	glBufferData(GL_ARRAY_BUFFER, dataSize*3*4, pointGrid, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void MarchingCubes::initTextures()
{	
	// DATA TEXTURE
	glGenTextures(1, &texData);
	glActiveTexture(GL_TEXTURE3);
	glEnable(GL_TEXTURE_3D);
	glBindTexture(GL_TEXTURE_3D, texData);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);	
	glTexImage3D( GL_TEXTURE_3D, 0, GL_ALPHA32F_ARB, dataWidth, dataHeight, dataDepth, 0, GL_ALPHA, GL_BYTE, dataField);
}

void MarchingCubes::setTextures()
{
	//MarchingCubesFactory::setTextures();

	glActiveTexture(GL_TEXTURE3);
	glEnable(GL_TEXTURE_3D);
	glBindTexture(GL_TEXTURE_3D, texData);
	glTexImage3D( GL_TEXTURE_3D, 0, GL_ALPHA32F_ARB, dataWidth, dataHeight, dataDepth, 0, GL_ALPHA, GL_BYTE, dataField);
}

void MarchingCubes::initUniforms( GLuint program )
{
	////Samplers assignment///
	glUniform1iARB(glGetUniformLocation(program, "dataFieldTex"), 1);
	glUniform1iARB(glGetUniformLocation(program, "edgeTableTex"), 2); 
    glUniform1iARB(glGetUniformLocation(program, "triTableTex"), 3); 

	////Uniforms parameters////
	//Initial isolevel
	glUniform1f(glGetUniformLocation(program, "isolevel"), treshold); 
	//Step in data 3D texture for gradient computation (lighting)
	glUniform3f(glGetUniformLocation(program, "dataStep"), 1.0f/dataWidth, 1.0f/dataHeight, 1.0f/dataDepth); 
	
	//Decal for each vertex in a marching cube
	float dx = 2.0f/dataWidth;
	float dy = 2.0f/dataHeight;
	float dz = 2.0f/dataDepth;
	glUniform3f(glGetUniformLocation(program, "vertDecals[0]"), 0.0f, 0.0f, 0.0f); 
	glUniform3f(glGetUniformLocation(program, "vertDecals[1]"), dx, 0.0f, 0.0f); 
	glUniform3f(glGetUniformLocation(program, "vertDecals[2]"), dx, dy, 0.0f); 
	glUniform3f(glGetUniformLocation(program, "vertDecals[3]"), 0.0f, dy, 0.0f); 
	glUniform3f(glGetUniformLocation(program, "vertDecals[4]"), 0.0f, 0.0f, dz); 
	glUniform3f(glGetUniformLocation(program, "vertDecals[5]"), dx, 0.0f, dz); 
	glUniform3f(glGetUniformLocation(program, "vertDecals[6]"), dx, dy, dz); 
	glUniform3f(glGetUniformLocation(program, "vertDecals[7]"), 0.0f, dy, dz); 
	
}