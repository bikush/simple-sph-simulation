#pragma once
#ifndef SPH_AABB_INTERACTOR_3D_H
#define SPH_AABB_INTERACTOR_3D_H

#include "SPHInteractor3d.h"
#include "GlmVec.h"

struct SPHParticle3d;

class SPHAABBInteractor3d : public SPHInteractor3d
{
	glm::vec3 min;
	glm::vec3 max;
	float distanceSquared;
	float distance;
	float dampening;

public:
	SPHAABBInteractor3d( glm::vec3 min, glm::vec3 max, float dampen = 1.0, float distance = 0.1 );

	void applyDensity( SPHParticle3d& other, glm::vec3 rvec );
	void applyForce( SPHParticle3d& other, glm::vec3 rvec );
	void enforceInteractor( SPHParticle3d& other, glm::vec3 rvec );
	glm::vec3 directionTo( SPHParticle3d& other );
	void draw();
};

#endif
