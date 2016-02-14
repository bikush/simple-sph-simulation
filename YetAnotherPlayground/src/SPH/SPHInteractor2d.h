#pragma once
#ifndef SPH_INTERACTOR_2D_H
#define SPH_INTERACTOR_2D_H

#include "Vector2D.h"

struct SPHParticle2d;

class SPHInteractor2d
{
public:
	virtual void applyDensity( SPHParticle2d& other, vec2f rvec )=0;
	virtual void applyForce( SPHParticle2d& other, vec2f rvec )=0;
	virtual void enforceInteractor( SPHParticle2d& other, vec2f rvec )=0;
	virtual vec2f directionTo( SPHParticle2d& other )=0;
	virtual void draw()=0;
};


#endif