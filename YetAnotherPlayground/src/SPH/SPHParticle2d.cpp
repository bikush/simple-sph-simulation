
#include "SPHParticle2d.h"

SPHParticle2d::SPHParticle2d()
{
	position = vec2f();
	velocity = vec2f();
	oldAcceleration = vec2f();
	mass = 0;
	density = 0;
	pressure = 0;
	force = vec2f();
}

SPHParticle2d::SPHParticle2d( vec2f pos, vec2f v, float m, float density )
{
	position = pos;
	velocity = v;
	oldAcceleration = vec2f();
	mass = m;
	this->density = 0;
	pressure = 0;
	force = vec2f();
	neighbours = std::vector<SPHParticle2d*>();
	colorGradient = vec2f();
	colorLaplacian = 0;
}

void SPHParticle2d::reset()
{
	density = 0;
	pressure = 0;
	force.x = 0;
	force.y = 0;
	neighbours.clear();

	colorGradient.x = 0;
	colorGradient.y = 0;
	colorLaplacian = 0;
}
