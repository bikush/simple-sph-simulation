#pragma once
#ifndef SPH_LINE_INTERACTOR_2D_H
#define SPH_LINE_INTERACTOR_2D_H

#include "SPHInteractor2d.h"
#include "GlmVec.h"

struct SPHParticle2d;

class SPHLineInteractor2d : public SPHInteractor2d
{
	glm::vec2 start;
	glm::vec2 end;
	glm::vec2 direction;
	float length;
	float lengthSquared;

	glm::vec2 last0;
	glm::vec2 last1;
		
public:
	SPHLineInteractor2d( glm::vec2 start, glm::vec2 end );

	void applyDensity( SPHParticle2d& other, glm::vec2 rvec );
	void applyForce( SPHParticle2d& other, glm::vec2 rvec );
	void enforceInteractor( SPHParticle2d& other, glm::vec2 rvec );
	glm::vec2 directionTo( SPHParticle2d& other );
	void draw();
};


#endif