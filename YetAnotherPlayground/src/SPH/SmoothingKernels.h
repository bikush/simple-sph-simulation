#pragma once
#ifndef SMOOTHING_KERNELS_H
#define SMOOTHING_KERNELS_H

#include "Vector2D.h"
#include "Vector3D.h"
#include <string>

enum KernelType
{
	POLY6,
	SPIKY,
	VISCOSITY,
	DEFAULT
};

enum SPHKernelUse
{
	BASE_KERNEL,		// Used in density update and surface tension forces
	PRESSURE_KERNEL,	// Used in pressure forces calculations
	VISCOSITY_KERNEL	// Used in viscosity forces calculation
};

class iKernel
{
protected:
	float baseFactor;
	float gradientFactor;
	float laplacianFactor;
	float h;

public:
	virtual float base( float r )=0;
	virtual vec2f gradient( vec2f r )=0;
	virtual vec3f gradient( vec3f r )=0;
	virtual float laplacian( float r )=0;
	virtual void  adjustSmoothingLength( float h )=0;
};

class KernelPoly6 : public iKernel
{	
	float hSquared;

public:
	KernelPoly6( float smoothingLength );

	float base( float r );
	vec2f gradient( vec2f r );
	vec3f gradient( vec3f r );
	float laplacian( float r );
	void  adjustSmoothingLength( float h );
};

class KernelSpiky : public iKernel
{
public:
	KernelSpiky( float smoothingLength );

	float base( float r );
	vec2f gradient( vec2f r );
	vec3f gradient( vec3f r );
	float laplacian( float r );
	void  adjustSmoothingLength( float h );
};

class KernelViscosity : public iKernel
{
public:
	KernelViscosity( float smoothingLength );

	float base( float r );
	vec2f gradient( vec2f r );
	vec3f gradient( vec3f r );
	float laplacian( float r );
	void  adjustSmoothingLength( float h );
};
/*
class KernelSplineGaussian : public iKernel
{
public:
	float base( float h, float r );
	vec2f gradient( float h, vec2f r );
	vec3f gradient( float h, vec3f r );
	float laplacian( float h, float r );
	//void  adjustSmoothingLength( float h );
};

class KernelDesburn : public iKernel
{
public:
	float base( float h, float r );
	vec2f gradient( float h, vec2f r );
	vec3f gradient( float h, vec3f r );
	float laplacian( float h, float r );
	//void  adjustSmoothingLength( float h );
};*/

class KernelBuilder
{
public:
	static iKernel* getKernel( std::string name, float smoothingLength );
	static iKernel* getKernel( KernelType type, float smoothingLength );
};

#endif