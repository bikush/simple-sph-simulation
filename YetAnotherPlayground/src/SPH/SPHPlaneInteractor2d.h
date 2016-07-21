#pragma once
#ifndef SPH_PLANE_INTERACTOR_2D_H
#define SPH_PLANE_INTERACTOR_2D_H

#include "SPHInteractor2d.h"
#include "GlmVec.h"

struct SPHParticle2d;

class SPHPlaneInteractor2d : public SPHInteractor2d
{
	glm::vec2 direction;
	glm::vec2 up;
	glm::vec2 start;
	float distanceSquared;
	float distance;

public:
	SPHPlaneInteractor2d( glm::vec2 startOffset, glm::vec2 direction, glm::vec2 upDir, float distance = 0.1 );
	SPHPlaneInteractor2d( glm::vec2 startOffset, glm::vec2 direction, float distance = 0.1 );

	void applyDensity( SPHParticle2d& other, glm::vec2 rvec );
	void applyForce( SPHParticle2d& other, glm::vec2 rvec );
	void enforceInteractor( SPHParticle2d& other, glm::vec2 rvec );
	glm::vec2 directionTo( SPHParticle2d& other );
	void draw();
};

#endif
