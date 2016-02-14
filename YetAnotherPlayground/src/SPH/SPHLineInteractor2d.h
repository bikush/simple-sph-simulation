#pragma once
#ifndef SPH_LINE_INTERACTOR_2D_H
#define SPH_LINE_INTERACTOR_2D_H

#include "SPHInteractor2d.h"
#include "Vector2D.h"

struct SPHParticle2d;

class SPHLineInteractor2d : public SPHInteractor2d
{
	vec2f start;
	vec2f end;
	vec2f direction;
	float length;
	float lengthSquared;

	vec2f last0;
	vec2f last1;
		
public:
	SPHLineInteractor2d( vec2f start, vec2f end );

	void applyDensity( SPHParticle2d& other, vec2f rvec );
	void applyForce( SPHParticle2d& other, vec2f rvec );
	void enforceInteractor( SPHParticle2d& other, vec2f rvec );
	vec2f directionTo( SPHParticle2d& other );
	void draw();
};


#endif