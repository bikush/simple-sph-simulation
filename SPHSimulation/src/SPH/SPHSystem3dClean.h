#pragma once
#ifndef SPHSYSTEM3D_CLEAN_H
#define SPHSYSTEM3D_CLEAN_H

#include "SPHParticle3d.h"
#include <vector>
#include <memory>

class iKernel;
class MarchingCubes;
class MarchingCubesShaded;
class SPHInteractor3d;
enum SPHKernelUse;
enum KernelType;
class PointDataVisualiser;

// This is the first and basic implementation. It has many weak and slow points.
// Replaced by SPHSystem3d.
// At least I hope so.
class SPHSystem3dClean
{
	std::vector<SPHParticleNeighbour3d> particles;
	int particleCount;

	std::vector<std::unique_ptr<SPHInteractor3d>> surfaces;

	float dWidth;
	float dHeight;
	float dDepth;

	float restDensity;
	float fluidConstantK;
	float viscosityConstant;
	float smoothingLength;
	float colorFieldTreshold;
	float surfaceTension;

	float unitRadius;

	float particleMass;

	bool useGravity;
	glm::vec3 gravityAcc;

	iKernel* kernel;
	iKernel* pressureKernel;
	iKernel* viscousKernel;	

	// Updates densities for both particles and generates neighbourhood data,
	// but only in the first particle (to avoid colisions in later calculations).
	void applyDensity(SPHParticleNeighbour3d& first, SPHParticleNeighbour3d& second );
	// Updates the forces for a given particle pair. It is asumed that the 
	// particles are neighbours and therefore proximity check is not made.
	void applyForces( SPHParticle3d& first, SPHParticle3d& second );
	// Updates the density against all surfaces (SPHInteractor).
	void applySurfaceDensity( SPHParticle3d& particle );
	// Updates the forces against all surfaces (SPHInteractor).
	void applySurfaceForces( SPHParticle3d& particle );
	
public:
	// Constructor with all necessary parameters and their default values. Does not include bounding surfaces.
	SPHSystem3dClean( float w = 30.0f, float h = 30.0f, float d = 30.0f, float density = 0.8f, float constantK = 10.0f, float contantMi = 0.6f, 
					float cfTreshold = 0.075f, float surfTension = 0.0015f, float mass = 0.3f, float smLen = 3.0f );
	// Constructor from a given mapped data file.
	SPHSystem3dClean( const char* file );
	~SPHSystem3dClean();

	// Main loop. Parameter is integration step. This method: resets particles, updates densities, updates
	// neighbourhood info, applies bounding surface densities, traverses neighbours for force update, updates
	// bounding surface forces, moves the particles with calculated forces, updates the grid.
	void animate( float dt );

	void setKernel( SPHKernelUse kernelUse, KernelType type );

	void addParticle( glm::vec3 position, glm::vec3 velocity );
	void addDistributedParticles( glm::vec3 start, glm::vec3 direction, glm::vec3 step );

	void addSurface( std::unique_ptr<SPHInteractor3d>& surface );
	void toggleSurface(int index);

	void draw( MarchingCubes* ms );
	void draw( MarchingCubesShaded* ms);
	void draw( PointDataVisualiser* pdv );
	void drawPoints();

	void setUseGravity( bool value );
	bool usesGravity();

	int getParticleCount();
	void clearAllParticles();

	float getRestDensity( );
	void setRestDensity( float density );
	
	float getK( );
	void setK( float k );

	float getViscosity( );
	void setViscosity( float viscosity );

	float getSmoothingLength( );
	void setSmoothingLength( float smLen );

	float getColorFieldTreshold();
	void setColorFieldTreshold( float cfTreshold );

	float getSurfaceTension();
	void setSurfaceTension( float sTension );
	
};

#endif