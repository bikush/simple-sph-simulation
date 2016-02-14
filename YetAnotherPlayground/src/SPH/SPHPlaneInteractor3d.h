#pragma once
#ifndef SPH_PLANE_INTERACTOR_3D_H
#define SPH_PLANE_INTERACTOR_3D_H

#include "SPHInteractor3d.h"
#include "Vector3D.h"

struct SPHParticle3d;

class SPHPlaneInteractor3d : public SPHInteractor3d
{
	vec3f up;
	vec3f start;
	float distanceSquared;
	float distance;

public:
	SPHPlaneInteractor3d( vec3f startOffset, vec3f upNormal, float distance = 0.1 );

	void applyDensity( SPHParticle3d& other, vec3f rvec );
	void applyForce( SPHParticle3d& other, vec3f rvec );
	void enforceInteractor( SPHParticle3d& other, vec3f rvec );
	vec3f directionTo( SPHParticle3d& other );
	void draw();
};

#endif
