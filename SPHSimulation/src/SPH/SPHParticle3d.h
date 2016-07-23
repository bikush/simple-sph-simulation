#pragma once
#ifndef SPHPARTICLE3D_H
#define SPHPARTICLE3D_H

#include "GlmVec.h"
#include "Utility.h"
#include <vector>

struct SPHParticle3d
{
	SPHParticle3d() :
		position(0), velocity(0), oldAcceleration(0),
		mass(0), density(0), pressure(0), force(0)
	{}

	SPHParticle3d(glm::vec3 pos, glm::vec3 v, float m, float density) :
		position(pos), velocity(v), oldAcceleration(0),
		mass(m), density(density), pressure(0), force(0),
		colorGradient(0), colorLaplacian(0)
	{}

	virtual void reset();

	glm::vec3 position;
	glm::vec3 velocity;
	glm::vec3 oldAcceleration;
	float mass;

	float density;
	float pressure;
	glm::vec3 force;
	float volume;

	glm::vec3 colorGradient;
	float colorLaplacian;
	
	//int padding[offsetFromBytes(128, sizeof_all2<glm::vec3, glm::vec3, glm::vec3, float, float, float, glm::vec3, float, glm::vec3, float>()) / 4 - 1];
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