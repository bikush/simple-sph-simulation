
#include "SPHParticle2d.h"

SPHParticle2d::SPHParticle2d()
{
	position = glm::vec2();
	velocity = glm::vec2();
	oldAcceleration = glm::vec2();
	mass = 0;
	density = 0;
	pressure = 0;
	force = glm::vec2();
}

SPHParticle2d::SPHParticle2d( glm::vec2 pos, glm::vec2 v, float m, float density )
{
	position = pos;
	velocity = v;
	oldAcceleration = glm::vec2();
	mass = m;
	this->density = 0;
	pressure = 0;
	force = glm::vec2();
	neighbours = std::vector<SPHParticle2d*>();
	colorGradient = glm::vec2();
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
