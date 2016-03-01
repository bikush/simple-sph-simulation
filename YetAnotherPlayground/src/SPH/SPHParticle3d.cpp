
#include "SPHParticle3d.h"

SPHParticle3d::SPHParticle3d()
{
	position = vec3f();
	velocity = vec3f();
	oldAcceleration = vec3f();
	mass = 0;
	density = 0;
	pressure = 0;
	force = vec3f();
}

SPHParticle3d::SPHParticle3d( vec3f pos, vec3f v, float m, float density )
{
	position = pos;
	velocity = v;
	oldAcceleration = vec3f();
	mass = m;
	this->density = 0;
	pressure = 0;
	force = vec3f();
	colorGradient = vec3f();
	colorLaplacian = 0;
}

void SPHParticle3d::reset()
{
	density = 0;
	pressure = 0;
	force = vec3f( 0, 0, 0);

	colorGradient = vec3f( 0, 0, 0 );
	colorLaplacian = 0;
}

void SPHParticleNeighbour3d::reset()
{
	SPHParticle3d::reset();
	neighbours.clear();
}
