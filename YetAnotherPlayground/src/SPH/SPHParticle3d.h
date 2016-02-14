#pragma once
#ifndef SPHPARTICLE3D_H
#define SPHPARTICLE3D_H

#include "Vector3D.h"
#include <vector>

struct SPHParticle3d
{
	SPHParticle3d();
	SPHParticle3d( vec3f pos, vec3f v, float m, float density );

	void reset();

	vec3f position;
	vec3f velocity;
	vec3f oldAcceleration;
	float mass;

	float density;
	float pressure;
	vec3f force;
	float volume;

	vec3f colorGradient;
	float colorLaplacian;

	std::vector<SPHParticle3d*> neighbours;

	 // Rounding up the structure to 128 bytes
#ifdef _DEBUG
	int padding[7];
#else
	int padding[8];
#endif
};

#endif