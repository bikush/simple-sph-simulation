#pragma once
#ifndef SPH_INTERACTOR_3D_H
#define SPH_INTERACTOR_3D_H

#include "GlmVec.h"

struct SPHParticle3d;

class SPHInteractor3d
{
protected:
	bool turnedOn;
public:
	SPHInteractor3d() : turnedOn(true) {};

	virtual void applyDensity( SPHParticle3d& other, glm::vec3 rvec )=0;
	virtual void applyForce( SPHParticle3d& other, glm::vec3 rvec )=0;
	virtual void enforceInteractor( SPHParticle3d& other, glm::vec3 rvec )=0;
	virtual glm::vec3 directionTo( SPHParticle3d& other )=0;
	virtual void draw()=0;
	
	void toggle()
	{
		turnedOn = !turnedOn;
	}
};


#endif