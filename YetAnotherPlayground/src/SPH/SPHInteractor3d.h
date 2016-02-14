#pragma once
#ifndef SPH_INTERACTOR_3D_H
#define SPH_INTERACTOR_3D_H

#include "Vector3D.h"

struct SPHParticle3d;

class SPHInteractor3d
{
protected:
	bool turnedOn;
public:
	SPHInteractor3d() : turnedOn(true) {};

	virtual void applyDensity( SPHParticle3d& other, vec3f rvec )=0;
	virtual void applyForce( SPHParticle3d& other, vec3f rvec )=0;
	virtual void enforceInteractor( SPHParticle3d& other, vec3f rvec )=0;
	virtual vec3f directionTo( SPHParticle3d& other )=0;
	virtual void draw()=0;
	
	void toggle()
	{
		turnedOn = !turnedOn;
	}
};


#endif