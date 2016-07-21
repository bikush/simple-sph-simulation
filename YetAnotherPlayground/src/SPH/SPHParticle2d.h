#pragma once
#ifndef SPHPARTICLE2D_H
#define SPHPARTICLE2D_H

#include "GlmVec.h"
#include <vector>

struct SPHParticle2d
{
	SPHParticle2d();
	SPHParticle2d( glm::vec2 pos, glm::vec2 v, float m, float density );

	void reset();

	glm::vec2 position;
	glm::vec2 velocity;
	glm::vec2 oldAcceleration;
	float mass;

	float density;
	float pressure;
	glm::vec2 force;

	glm::vec2 colorGradient;
	float colorLaplacian;

	std::vector<SPHParticle2d*> neighbours;

	int padding[9]; // This helps...
};

#endif