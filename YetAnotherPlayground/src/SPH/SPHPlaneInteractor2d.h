#pragma once
#ifndef SPH_PLANE_INTERACTOR_2D_H
#define SPH_PLANE_INTERACTOR_2D_H

#include "SPHInteractor2d.h"
#include "Vector2D.h"

struct SPHParticle2d;

class SPHPlaneInteractor2d : public SPHInteractor2d
{
	vec2f direction;
	vec2f up;
	vec2f start;
	float distanceSquared;
	float distance;

public:
	SPHPlaneInteractor2d( vec2f startOffset, vec2f direction, vec2f upDir, float distance = 0.1 );
	SPHPlaneInteractor2d( vec2f startOffset, vec2f direction, float distance = 0.1 );

	void applyDensity( SPHParticle2d& other, vec2f rvec );
	void applyForce( SPHParticle2d& other, vec2f rvec );
	void enforceInteractor( SPHParticle2d& other, vec2f rvec );
	vec2f directionTo( SPHParticle2d& other );
	void draw();
};

#endif
