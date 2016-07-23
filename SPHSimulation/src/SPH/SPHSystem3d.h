#pragma once
#ifndef SPHSYSTEM3D_H
#define SPHSYSTEM3D_H

#include "SPHParticle3d.h"
#include "SmoothingKernels.h"
#include <vector>
#include <memory>
#include <glm\gtx\norm.hpp>

//class iKernel;
class MarchingCubes;
class MarchingCubesBasic;
class SPHInteractor3d;
class PointDataVisualiser;
class MarchingCubesShaded;

struct SPHPair
{
	SPHPair( SPHParticle3d& f, SPHParticle3d& s, glm::vec3 r ) : first(f), second(s), rvec(r)
	{}

	SPHPair& operator=( SPHPair& other )
	{
		first = other.first;
		second = other.second;
		rvec = other.rvec;
		return *this;
	}

	SPHParticle3d& first;
	SPHParticle3d& second;
	glm::vec3 rvec;	
};

class SPHSystem3d
{
	std::vector<SPHParticle3d> particles;
	
	std::vector< std::vector< int > > grid;
	int gridOffsets[13];
	int gridWidth;
	int gridHeight;
	int gridDepth;

	std::vector< SPHPair > pairs;

	std::vector<std::unique_ptr<SPHInteractor3d>> surfaces;
	int particleCount;

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

	// Updates densities for both particles and generates neighbourhood data,
	// but only in the first particle (to avoid colisions in later calculations).
	void applyDensity( SPHParticle3d& first, SPHParticle3d& second );
	// Updates the forces for a given particle pair. It is asumed that the 
	// particles are neighbours and therefore proximity check is not made.
	void applyForces( SPHParticle3d& first, SPHParticle3d& second );
	void applyForces( SPHParticle3d& first, SPHParticle3d& second, glm::vec3 rvec );
	// Updates the density against all surfaces (SPHInteractor).
	void applySurfaceDensity( SPHParticle3d& particle );
	// Updates the forces against all surfaces (SPHInteractor).
	void applySurfaceForces( SPHParticle3d& particle );


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

	float hSquared;
	float kp6baseFactor;
	float kp6gradientFactor;
	float kp6laplacianFactor;
	float ksbaseFactor;
	float ksgradientFactor;
	float kslaplacianFactor;
	float kvbaseFactor;
	float kvgradientFactor;
	float kvlaplacianFactor;

	void adjustSmoothingLength( float smlen );

	inline float kp6base( float rSq )
	{
		return kp6baseFactor * ( pow(hSquared - rSq, 3) );
	}
	inline glm::vec3 kp6gradient( glm::vec3 rvec )
	{
		return rvec*( kp6gradientFactor*pow(hSquared - glm::length2( rvec ), 2) );		
	}
	inline float kp6laplacian( float rSq )
	{
		return kp6laplacianFactor * (hSquared - rSq) * ( -0.75f*(hSquared - rSq) + rSq ); // 3PI is experimental constant
	}
	inline float ksbase( float r )
	{
		return ksbaseFactor * pow(smoothingLength-r, 3);		
	}
	inline glm::vec3 ksgradient( glm::vec3 rvec )
	{
		float r = glm::length( rvec );
		return rvec *( ksgradientFactor * pow(smoothingLength-r, 2) / r );
	}
	inline float kslaplacian( float r )
	{
		r = r<0.0001f ? 0.0001f : r;		
		return kslaplacianFactor *(hSquared/r-3*smoothingLength+2*r);
	}
	inline float kvbase( float r )
	{		
		float rh = r/smoothingLength;
		return kvbaseFactor * ( -(rh*rh*rh/2) + rh*rh + 1/(rh*2) - 1  );	
	}
	inline glm::vec3 kvgradient( glm::vec3 rvec )
	{
		float r = glm::length( rvec );
		float rSq = r*r;
		return  rvec*(( kvgradientFactor * ( -(3*rSq/(2*hSquared*smoothingLength)) + 2*r/(hSquared) - smoothingLength/(2*rSq)  ) )/r);
	}
	inline float kvlaplacian( float r )
	{
		return kvlaplacianFactor * ( smoothingLength-r ) / smoothingLength; // /smoothingLength; this is not in the original kernel
	}

public:
	// Constructor with all necessary parameters and their default values. Does not include bounding surfaces.
	SPHSystem3d( float w = 30.0f, float h = 30.0f, float d = 30.0f, float density = 0.8f, float constantK = 10.0f, float contantMi = 0.6f, 
					float cfTreshold = 0.075f, float surfTension = 0.0015f, float mass = 0.3f, float smLen = 3.0f );
	// Constructor from a given mapped data file.
	SPHSystem3d( const char* file );
	~SPHSystem3d();

	// Main loop. Parameter is integration step. This method: resets particles, updates densities, updates
	// neighbourhood info, applies bounding surface densities, traverses neighbours for force update, updates
	// bounding surface forces, moves the particles with calculated forces, updates the grid.
	void animate( float dt );

	void addParticle( glm::vec3 position, glm::vec3 velocity );
	void addDistributedParticles( glm::vec3 start, glm::vec3 direction, glm::vec3 step );

	void addSurface(std::unique_ptr<SPHInteractor3d>& surface );
	void toggleSurface( int index );

	void draw( MarchingCubes* ms );
	void draw( MarchingCubesBasic* ms );
	void draw( MarchingCubesShaded* ms );
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
	
	void paramOutput();
	
};

#endif