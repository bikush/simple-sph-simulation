#pragma once
#ifndef SPHPARTICLE3D_H
#define SPHPARTICLE3D_H

#include "Vector3D.h"
#include "Utility.h"
#include <vector>

struct SPHParticle3d
{
	SPHParticle3d() :
		position(0), velocity(0), oldAcceleration(0),
		mass(0), density(0), pressure(0), force(0)
	{}

	SPHParticle3d(vec3f pos, vec3f v, float m, float density) :
		position(pos), velocity(v), oldAcceleration(0),
		mass(m), density(density), pressure(0), force(0),
		colorGradient(0), colorLaplacian(0)
	{}

	virtual void reset();

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
	
	//int padding[offsetFromBytes(128, sizeof_all2<vec3f, vec3f, vec3f, float, float, float, vec3f, float, vec3f, float>()) / 4 - 1];
	// Rounding up the structure to 128 bytes
#ifdef _DEBUG
	int padding[11];
#else
	int padding[8];
#endif
};

struct SPHParticleNeighbour3d : SPHParticle3d
{
	using SPHParticle3d::SPHParticle3d;

	std::vector<SPHParticleNeighbour3d*> neighbours;

	void reset();
};

#endif