#pragma once
#ifndef SPHSYSTEM2D_H
#define SPHSYSTEM2D_H

#include "SPHParticle2d.h"
#include "SmoothingKernels.h"
#include <vector>

//class iKernel;
class MarchingSquaresBase;
class SPHInteractor2d;

class SPHSystem2d
{
	std::vector<SPHParticle2d> particles;
//	glm::vec2 *positions;
//	int positionsSize;
	
	std::vector< std::vector< int > > grid;
	int gridWidth;
	int gridHeight;

	std::vector<SPHInteractor2d*> surfaces;
	int particleCount;

	float dWidth;
	float dHeight;

	float restDensity;
	float fluidConstantK;
	float viscosityConstant;
	float smoothingLength;
	float colorFieldTreshold;
	float surfaceTension;

	float unitRadius;

	float particleMass;

	bool useGravity;
	glm::vec2 gravityAcc;

	iKernel* kernel;
	iKernel* pressureKernel;
	iKernel* viscousKernel;	

	// Updates densities for both particles and generates neighbourhood data,
	// but only in the first particle (to avoid colisions in later calculations).
	void applyDensity( SPHParticle2d& first, SPHParticle2d& second );
	// Updates the forces for a given particle pair. It is asumed that the 
	// particles are neighbours and therefore proximity check is not made.
	void applyForces( SPHParticle2d& first, SPHParticle2d& second );
	// Updates the density against all surfaces (SPHInteractor).
	void applySurfaceDensity( SPHParticle2d& particle );
	// Updates the forces against all surfaces (SPHInteractor).
	void applySurfaceForces( SPHParticle2d& particle );


	// Recalculates grid dimensions and resizes the vectors if necessary.
	void createGrid();
	// Reposition all existing particles. Assumes the grid is clear.
	void fillGrid();
	// Clears all grid vectors. Used prior to fillGrid method.
	void clearGrid();
	void putParticleIntoGrid( int particleIndex );

	// Traversal of the grid for initial density calculation. ApplyDensity is
	// called on valid particle pairs. This also generates neighbourhood lists!
	void gridDensityUpdate( );
	// Matches every particle to every other particle for density and neighbourhood
	// update.
	void densityUpdate();

public:
	// Constructor with all necessary parameters and their default values. Does not include bounding surfaces.
	SPHSystem2d( float w = 30.0f, float h = 30.0f, float density = 0.8f, float constantK = 10.0f, float contantMi = 0.6f, 
					float cfTreshold = 0.075f, float surfTension = 0.0015f, float mass = 0.3f, float smLen = 3.0f );
	// Constructor from a given mapped data file.
	SPHSystem2d( const char* file );
	~SPHSystem2d();

	// Main loop. Parameter is integration step. This method: resets particles, updates densities, updates
	// neighbourhood info, applies bounding surface densities, traverses neighbours for force update, updates
	// bounding surface forces, moves the particles with calculated forces, updates the grid.
	void animate( float dt );

	void setKernel( SPHKernelUse kernelUse, KernelType type );

	void addParticle( glm::vec2 position, glm::vec2 velocity );
	void addDistributedParticles( glm::vec2 start, glm::vec2 direction, glm::vec2 step );

	void addSurface( SPHInteractor2d* surface );

	void draw( MarchingSquaresBase* ms );
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
	
	void paramOutput();
	
};

#endif