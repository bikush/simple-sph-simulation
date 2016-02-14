#pragma once
#ifndef SPH_AABB_INTERACTOR_3D_H
#define SPH_AABB_INTERACTOR_3D_H

#include "SPHInteractor3d.h"
#include "Vector3D.h"

struct SPHParticle3d;

class SPHAABBInteractor3d : public SPHInteractor3d
{
	vec3f min;
	vec3f max;
	float distanceSquared;
	float distance;
	float dampening;

public:
	SPHAABBInteractor3d( vec3f min, vec3f max, float dampen = 1.0, float distance = 0.1 );

	void applyDensity( SPHParticle3d& other, vec3f rvec );
	void applyForce( SPHParticle3d& other, vec3f rvec );
	void enforceInteractor( SPHParticle3d& other, vec3f rvec );
	vec3f directionTo( SPHParticle3d& other );
	void draw();
};

#endif
