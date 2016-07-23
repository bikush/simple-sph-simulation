#pragma once
#ifndef FRUSTUM_H
#define FRUSTUM_H

#include "Vector3D.h"

class Frustum {

public:
	float NCP;
	float FCP;

	float FOVx;
	float FOVy;

	float aspectRatio; //aspect ratio	

	float width;
	float height;

	//vec3f G;
	vec3f O;
	vec3f tO;
	vec3f rO;
	vec3f look;

	Frustum();
	Frustum( const char* paramFile );
	Frustum( float nCP, float fCP, float fOV, float winW, float winH, vec3f ociste, vec3f glediste );

	void setEyes( vec3f ociste, vec3f glediste );
	void setEyesOffset( vec3f pomak );
	void setEyesAngle( float x, float y, float z );
	void offsetEyesAngle( float x, float y, float z );
	void offsetEyesOffset( float x, float y, float z );
	void setFOV( float );
	void setWidthHeight( float, float );

	void setupGLProjection();
	void setupCamera();

	void CalculateFrustum();
	bool SphereInFrustum(float x, float y, float z, float radius);
	void NormalizePlane(float frustum[6][4], int side);

	void moveForward();
	void moveBackward();
	
private:
	float m_Frustum[6][4];
};



#endif