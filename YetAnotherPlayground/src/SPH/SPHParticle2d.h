#pragma once
#ifndef SPHPARTICLE2D_H
#define SPHPARTICLE2D_H

#include "Vector2D.h"
#include <vector>

struct SPHParticle2d
{
	SPHParticle2d();
	SPHParticle2d( vec2f pos, vec2f v, float m, float density );

	void reset();

	vec2f position;
	vec2f velocity;
	vec2f oldAcceleration;
	float mass;

	float density;
	float pressure;
	vec2f force;

	vec2f colorGradient;
	float colorLaplacian;

	std::vector<SPHParticle2d*> neighbours;

	int padding[9]; // This helps...
};

#endif