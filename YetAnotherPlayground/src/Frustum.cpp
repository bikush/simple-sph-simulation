#include "Frustum.h"
#include <math.h>
#include <windows.h>
#include <gl\glew.h>
#include "Matrix.h"
#include "MappedData.h"
#include "Utility.h"
#include <glm\geometric.hpp>

enum FrustumSide
{
	RIGHT	= 0,		// The RIGHT side of the frustum
	LEFT	= 1,		// The LEFT	 side of the frustum
	BOTTOM	= 2,		// The BOTTOM side of the frustum
	TOP		= 3,		// The TOP side of the frustum
	BACK	= 4,		// The BACK	side of the frustum
	FRONT	= 5			// The FRONT side of the frustum
}; 

enum PlaneData
{
	A = 0,				// The X value of the plane's normal
	B = 1,				// The Y value of the plane's normal
	C = 2,				// The Z value of the plane's normal
	D = 3				// The distance the plane is from the origin
};

Frustum::Frustum()
{

}

Frustum::Frustum( const char* paramFile )
{
	MappedData md( paramFile );
	NCP = md.getData("clip","ncp").getFloat();
	FCP = md.getData("clip","fcp").getFloat();
	
	setWidthHeight(
			md.getData("window", "width").getFloat(),
			md.getData("window", "height").getFloat()
		);
	setFOV( md.getData("view","fov").getFloat() );
	setEyes(
			md.getData("view","eyesPosition").getVec3f(),
			md.getData("view","eyesLookAt").getVec3f()
		);
}

Frustum::Frustum( float nCP, float fCP, float fOV, float winW, float winH, vec3f ociste, vec3f glediste )
{
	NCP = nCP;
	FCP = fCP;
	setWidthHeight( winW, winH );
	setFOV( fOV );
	setEyes( ociste, glediste );
}

void Frustum::setEyes( vec3f ociste, vec3f glediste )
{
	look = glm::normalize(glediste - ociste );

	float kutY = 0;
	float kutZ = 0;
	float kutX = 0;
	rO = vec3f(kutX, kutY, kutZ);

	//G = vec3f(0,0,0);
	O = ociste;
	tO = vec3f(0,0,0);
}

void Frustum::setEyesOffset( vec3f pomak )
{
	tO = pomak;
}

void Frustum::setEyesAngle( float x, float y, float z )
{
	rO = vec3f(x,y,z);
}

void Frustum::offsetEyesAngle( float x, float y, float z )
{
	rO += vec3f(x,y,z);	

	/*float dx = tO.x * cos( rO.x );
	float dy = tO.y * cos( rO.y );
	float dz = tO.z * cos( rO.z );

	O += vec3f(dx, dy, dz);	
	tO=vec3f(0,0,0);*/
}

void Frustum::moveForward()
{
	// TODO
}

void Frustum::moveBackward()
{
	// TODO
}

void Frustum::offsetEyesOffset( float x, float y, float z )
{	
	O += vec3f(x, y, z);	
}

void Frustum::setupCamera()
{
	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();		
	glRotatef( rO.x, 1.0, 0.0, 0.0);
	glRotatef( rO.y, 0.0, 1.0, 0.0);
	glRotatef( rO.z, 0.0, 0.0, 1.0);
	glTranslatef( -O.x, -O.y, -O.z );
}

void Frustum::setupGLProjection()
{
	/*glMatrixMode(GL_PROJECTION);				
	glLoadIdentity();							
	gluPerspective(FOVy, aspectRatio, NCP, FCP); */	
}

void Frustum::setFOV( float fOV )
{	
	FOVx = fOV;
	FOVy = angleFromRadians( atan( tan( angleToRadians(FOVx/2) ) / aspectRatio ) ); //WARNING: maybe *2, maybe not
}

void Frustum::setWidthHeight( int winW, int winH )
{
	width = winW;
	height = winH;
	aspectRatio = (float)winW / (float)winH;
}

void Frustum::NormalizePlane(float frustum[6][4], int side)
{
	float magnitude = (float)sqrt( frustum[side][A] * frustum[side][A] + 
								   frustum[side][B] * frustum[side][B] + 
								   frustum[side][C] * frustum[side][C] );

	frustum[side][A] /= magnitude;
	frustum[side][B] /= magnitude;
	frustum[side][C] /= magnitude;
	frustum[side][D] /= magnitude; 
}

// TODO: implement with actual matrices from th camera
void Frustum::CalculateFrustum()
{    
	float   proj[16];						
	float   modl[16];						
	float*  clip;						

	glGetFloatv( GL_PROJECTION_MATRIX, proj );
	glGetFloatv( GL_MODELVIEW_MATRIX, modl );

	Matrix m = Matrix(4, 4, modl);
	m *= Matrix(4,4,proj);
	clip = m.copy();

	// This will extract the RIGHT side of the frustum
	m_Frustum[RIGHT][A] = clip[ 3] - clip[ 0];
	m_Frustum[RIGHT][B] = clip[ 7] - clip[ 4];
	m_Frustum[RIGHT][C] = clip[11] - clip[ 8];
	m_Frustum[RIGHT][D] = clip[15] - clip[12];

	// Now that we have a normal (A,B,C) and a distance (D) to the plane,
	// we want to normalize that normal and distance.

	// Normalize the RIGHT side
	NormalizePlane(m_Frustum, RIGHT);

	// This will extract the LEFT side of the frustum
	m_Frustum[LEFT][A] = clip[ 3] + clip[ 0];
	m_Frustum[LEFT][B] = clip[ 7] + clip[ 4];
	m_Frustum[LEFT][C] = clip[11] + clip[ 8];
	m_Frustum[LEFT][D] = clip[15] + clip[12];

	// Normalize the LEFT side
	NormalizePlane(m_Frustum, LEFT);

	// This will extract the BOTTOM side of the frustum
	m_Frustum[BOTTOM][A] = clip[ 3] + clip[ 1];
	m_Frustum[BOTTOM][B] = clip[ 7] + clip[ 5];
	m_Frustum[BOTTOM][C] = clip[11] + clip[ 9];
	m_Frustum[BOTTOM][D] = clip[15] + clip[13];

	// Normalize the BOTTOM side
	NormalizePlane(m_Frustum, BOTTOM);

	// This will extract the TOP side of the frustum
	m_Frustum[TOP][A] = clip[ 3] - clip[ 1];
	m_Frustum[TOP][B] = clip[ 7] - clip[ 5];
	m_Frustum[TOP][C] = clip[11] - clip[ 9];
	m_Frustum[TOP][D] = clip[15] - clip[13];

	// Normalize the TOP side
	NormalizePlane(m_Frustum, TOP);

	// This will extract the BACK side of the frustum
	m_Frustum[BACK][A] = clip[ 3] - clip[ 2];
	m_Frustum[BACK][B] = clip[ 7] - clip[ 6];
	m_Frustum[BACK][C] = clip[11] - clip[10];
	m_Frustum[BACK][D] = clip[15] - clip[14];

	// Normalize the BACK side
	NormalizePlane(m_Frustum, BACK);

	// This will extract the FRONT side of the frustum
	m_Frustum[FRONT][A] = clip[ 3] + clip[ 2];
	m_Frustum[FRONT][B] = clip[ 7] + clip[ 6];
	m_Frustum[FRONT][C] = clip[11] + clip[10];
	m_Frustum[FRONT][D] = clip[15] + clip[14];

	// Normalize the FRONT side
	NormalizePlane(m_Frustum, FRONT);

	delete clip;
}

bool Frustum::SphereInFrustum( float x, float y, float z, float radius )
{
	for(int i = 0; i < 6; i++ )	
	{
		if( m_Frustum[i][A] * x + m_Frustum[i][B] * y + m_Frustum[i][C] * z + m_Frustum[i][D] <= -2*radius )
		{
			return false;
		}
	}
	return true;
}

