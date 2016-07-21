#pragma once
#ifndef SPH_INTERACTOR_2D_H
#define SPH_INTERACTOR_2D_H

#include "GlmVec.h"

struct SPHParticle2d;

class SPHInteractor2d
{
public:
	virtual void applyDensity( SPHParticle2d& other, glm::vec2 rvec )=0;
	virtual void applyForce( SPHParticle2d& other, glm::vec2 rvec )=0;
	virtual void enforceInteractor( SPHParticle2d& other, glm::vec2 rvec )=0;
	virtual glm::vec2 directionTo( SPHParticle2d& other )=0;
	virtual void draw()=0;
};


#endif