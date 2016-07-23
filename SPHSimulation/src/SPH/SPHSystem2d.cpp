
#include "SPHSystem2d.h"
//#include "SmoothingKernels.h"
#include "MarchingSquaresBase.h"
#include "SPHInteractor2d.h"
#include "SPHInteractor2dFactory.h"
#include "MappedData.h"
#include <gl\glew.h>
#include <iostream>
#include <math.h>
#include <glm\common.hpp>
#include <glm\gtx\norm.hpp>

using namespace std;

// Creates a SPH System 2d from base parameters. Default kernels are set and 
// no bounding surfaces are defined, to do so, appropriate methods should be called.
SPHSystem2d::SPHSystem2d( float w, float h, float density, float constantK, float constantMi, 
							float cfTreshold, float surfTension,  float mass, float smLen )
{
	particles = vector<SPHParticle2d>();
	particleCount = 0;
	//positions = new glm::vec2[200];
	//positionsSize = 200;

	dWidth = w;
	dHeight = h;

	restDensity = density;
	fluidConstantK = constantK;
	viscosityConstant = constantMi;
	colorFieldTreshold = 0.075f;
	colorFieldTreshold *= cfTreshold;
	surfaceTension = surfTension;	
	particleMass = mass;
	unitRadius = sqrt( particleMass / (restDensity*PI) );

	useGravity = true;
	gravityAcc =  glm::vec2( 0, -0.981*10 );	

	smoothingLength = smLen;
	kernel = KernelBuilder::getKernel( "KernelPoly6", smoothingLength );
	pressureKernel = KernelBuilder::getKernel( "KernelPoly6", smoothingLength );
	viscousKernel = KernelBuilder::getKernel( "KernelPoly6", smoothingLength );
	
	grid = vector< vector< int > >();
	gridWidth = -1;
	gridHeight = -1;
	createGrid();
}

// Creates a SPH System 2d from a mapped data file. The file must contain the following
// groups and fields:
//  - grid: width (float), height (float), surfaces (surface group names)
//  - fluid: density, k, viscosity, colorFieldTreshold, surfaceTension, unitMass (all floats), gravity (two floats)
//  - kernel: smoothingLength (float), base (string), pressure (string), viscous (string)
//  - additional groups describing bounding surfaces provided by SPHInteractor2dFactory
SPHSystem2d::SPHSystem2d( const char* file )
{
	MappedData map( file );
	particles = vector<SPHParticle2d>();	
	particleCount = 0;

	dWidth = map.getData( "grid", "width" ).get<float>();
	dHeight = map.getData( "grid", "height" ).get<float>();
	vector<string> surfaceNames = map.getData( "grid", "surfaces" ).getVector<string>();
	for( string sName : surfaceNames )
	{
		surfaces.push_back( SPHInteractor2dFactory::getInteractor( sName, &map ) );
	}

	restDensity = map.getData( "fluid", "density" ).get<float>();
	fluidConstantK = map.getData( "fluid", "k" ).get<float>();
	viscosityConstant = map.getData( "fluid", "viscosity" ).get<float>();
	colorFieldTreshold = map.getData( "fluid", "colorFieldTreshold" ).get<float>();
	colorFieldTreshold *= colorFieldTreshold;
	surfaceTension = map.getData( "fluid", "surfaceTension" ).get<float>();
	particleMass = map.getData( "fluid", "unitMass" ).get<float>();
	gravityAcc =  map.getData( "fluid", "gravity" ).getVec2() * particleMass;

	smoothingLength = map.getData( "kernel", "smoothingLength" ).get<float>();
	kernel = KernelBuilder::getKernel( map.getData("kernel", "base").getStringData(), smoothingLength );
	pressureKernel = KernelBuilder::getKernel( map.getData("kernel", "pressure").getStringData(), smoothingLength );
	viscousKernel =  KernelBuilder::getKernel( map.getData("kernel", "viscous" ).getStringData(), smoothingLength );
	
	unitRadius = sqrt( particleMass / (restDensity*PI) );
	useGravity = true;	

	grid = vector< vector< int > >();
	gridWidth = -1;
	gridHeight = -1;
	createGrid();
}


SPHSystem2d::~SPHSystem2d()
{
	// No special cleanup needed
	/*particles.clear();	
	grid.clear();*/
}

void SPHSystem2d::clearGrid()
{
	for(int i=0; i< gridHeight*gridWidth; i++)
	{
		grid[i].clear();
	}
}

void SPHSystem2d::createGrid()
{
	int newGridHeight = (int)ceil( dHeight / smoothingLength );
	int newGridWidth = (int)ceil( dWidth / smoothingLength );
	// If the grid dimensions change, rebuild the grid vectors
	if( gridWidth != newGridWidth || gridHeight!= newGridHeight )
	{
		gridWidth = newGridWidth;
		gridHeight = newGridHeight;
		grid.clear();
		for(int i=0; i< gridHeight*gridWidth; i++)
		{
			grid.push_back( vector< int >() );
		}
	}else	// else just clear the existing grid vectors
	{
		clearGrid();
	}
	fillGrid();	
}

void SPHSystem2d::fillGrid( )
{
	for( int i = 0; i<particleCount; i++ )
	{
		putParticleIntoGrid( i );
	}
}

void SPHSystem2d::putParticleIntoGrid( int particleIndex )
{
	// Calculate grid index
	glm::vec2 position = (particles[ particleIndex ]).position;
	int x = int( position.x * gridWidth / dWidth );
	int y = int( position.y * gridHeight / dHeight );
	if( x >= gridWidth ) x=gridWidth-1;
	if( y >= gridHeight ) y=gridHeight-1;

	grid[ y*gridWidth + x ].push_back( particleIndex );
}


void SPHSystem2d::setKernel( SPHKernelUse kernelUse, KernelType type )
{
	iKernel::unique* old;
	switch( kernelUse )
	{	
	case PRESSURE_KERNEL:
		old = &pressureKernel;
		break;
	case VISCOSITY_KERNEL:
		old = &viscousKernel;
		break;
	case BASE_KERNEL: 
	default:	// Assumed base kernel setting if wrong kernel type
		old = &kernel;
		break;
	}
	(*old) =  KernelBuilder::getKernel( type, smoothingLength );
}

void SPHSystem2d::addParticle( glm::vec2 position, glm::vec2 velocity )
{
	position = glm::clamp( position, glm::vec2(0,0), glm::vec2( dWidth, dHeight ) );
	particles.push_back( SPHParticle2d( position, velocity, particleMass, restDensity ) );
	putParticleIntoGrid( particleCount );
	particleCount++;	

/*	if( particleCount > positionsSize )
	{
		positionsSize *= 1.5;
		delete [] positions;
		positions = new glm::vec2[positionsSize];
	}*/
}

void SPHSystem2d::addSurface(std::unique_ptr<SPHInteractor2d>& surface )
{
	surfaces.push_back( std::move(surface) );
}

void SPHSystem2d::applyDensity( SPHParticle2d& first, SPHParticle2d& second )
{
	glm::vec2 rvec = first.position - second.position;
	float r = glm::length( rvec );
	if( r < smoothingLength )
	{
		float additionalDensity = particleMass * kernel->base(r);
		first.density += additionalDensity;
		second.density += additionalDensity;
		first.neighbours.push_back( &second );
	}
}

// NOTE: Assume this is called on neighbourhood data. No smoothing check is made.
void SPHSystem2d::applyForces( SPHParticle2d& first, SPHParticle2d& second )
{
	glm::vec2 rvec = (first.position - second.position);
	float r = glm::length( rvec );
	
	if( r <= 0.000001 ) 
	{
		rvec = glm::vec2( 0.707, 0.707 );
		r = 1;
	}
	/*
	glm::vec2 commonPressureInfluence = 
		pressureKernel->gradient( rvec ) * 
		( 
			particleMass * 
			(
				second.pressure + first.pressure 
			) / 2.0
		); /* unified */
	glm::vec2 commonPressureInfluence = 
		pressureKernel->gradient( rvec ) * 
		( 
			particleMass * 
			(	
				second.pressure / ( second.density*second.density ) +
				first.pressure / (first.density*first.density)
			)
		);/* by definition */
		
	// viscosity forces
		
	/*glm::vec2 commonViscousInfluence = (second.velocity - first.velocity) * 
		(viscosityConstant * particleMass * viscousKernel->laplacian( r )); /* unified */
	glm::vec2 commonViscousInfluence = 
		(second.velocity - first.velocity) * 
		(
			viscosityConstant * particleMass * 
			viscousKernel->laplacian( r ) / 
			(second.density*first.density)
		);/* by definition */

	first.force += ( commonPressureInfluence + commonViscousInfluence) / second.density;
	second.force += (-commonPressureInfluence - commonViscousInfluence) / first.density;

	glm::vec2 commonColorGradient = kernel->gradient( rvec ) * particleMass;
	first.colorGradient += commonColorGradient / second.density;
	second.colorGradient += commonColorGradient / first.density;

	float commonColorLaplacian = kernel->laplacian( r ) * particleMass;
	first.colorLaplacian += commonColorLaplacian / second.density;
	second.colorLaplacian += commonColorLaplacian / first.density;
		
}

void SPHSystem2d::applySurfaceDensity( SPHParticle2d& particle )
{
	glm::vec2 rvec;
	float r;
	for (size_t surf = 0, surfLen = surfaces.size(); surf < surfLen; surf++)
	{
		rvec = surfaces[surf]->directionTo( particle );
		r = glm::length( rvec );
		if( r < smoothingLength )
		{
			particle.density += particleMass * kernel->base(r);
		}
	}
}

void SPHSystem2d::applySurfaceForces( SPHParticle2d& particle )
{
	glm::vec2 rvec;
	float r;
	for (size_t surf = 0, surfLen = surfaces.size(); surf < surfLen; surf++)
	{
		rvec = surfaces[surf]->directionTo( particle );
		r = glm::length( rvec );
		if( r < smoothingLength )
		{
			// pressure
			particle.force -= pressureKernel->gradient( rvec ) * particleMass * particle.pressure / particle.density;
			// viscosity
			particle.force += ( -particle.velocity ) * (viscousKernel->laplacian( r ) * viscosityConstant * particleMass / particle.density);
		}
	}
}

void SPHSystem2d::gridDensityUpdate( )
{
	for( int x=0; x<gridWidth; x++)
	{
	for( int y=0; y<gridHeight; y++)
	{	
		int index = y*gridWidth+x;
		int particlesInGrid = grid[index].size();
		if( particlesInGrid < 1 ) continue;

		int indexRight = index+1;
		int particlesRight = x+1 == gridWidth ? -1 : grid[indexRight].size();

		int indexUp = index+gridWidth;
		int particlesUp = y+1 == gridHeight ? -1 : grid[indexUp].size();

		int indexDiagonal = indexUp+1;
		int particlesDiagonal = y+1 == gridHeight || x+1 == gridWidth ? -1 : grid[indexDiagonal].size();

		int indexDown = index-gridWidth+1;
		int particlesDown = y-1 == -1 || x+1 == gridWidth ? -1 : grid[indexDown].size();

		// Go through the current grid, C current cell, V cell to visit, X ignored cell
		// X | V | V
		//-----------
		// X | C | V
		//-----------
		// X | X | V
		for(int j=0; j<particlesInGrid; j++)
		{				
			SPHParticle2d& particle = particles[ grid[index][j] ];
			particle.density += particleMass;

			// Visit rest of the current grid
			for(int k=j+1; k<particlesInGrid; k++)
			{
				applyDensity( particle, particles[ grid[index][k] ] );
			}
			// Visit right grid, (particlesRight == -1) when there is no grid to the right
			for(int k=0; k<particlesRight; k++)
			{
				applyDensity( particle, particles[ grid[indexRight][k] ] );
			}
			// Visit up grid, (particlesUp == -1) when there is no grid above
			for(int k=0; k<particlesUp; k++)
			{
				applyDensity( particle, particles[ grid[indexUp][k] ] );
			}
			// Visit diagonal grid, (particlesDiagonal == -1) when there is no grid diagonaly
			for(int k=0; k<particlesDiagonal; k++)
			{
				applyDensity( particle, particles[ grid[indexDiagonal][k] ] );
			}
			// Visit down diagonal grid, (particlesDown == -1) when there is no grid diagonaly down
			for(int k=0; k<particlesDown; k++)
			{
				applyDensity( particle, particles[ grid[indexDown][k] ] );
			}

			applySurfaceDensity( particle );

			particle.pressure = fluidConstantK * (particle.density - restDensity );
		}
	}
	}
}

void SPHSystem2d::densityUpdate()
{
	for(int i=0; i<particleCount; i++)
	{		
		// particle - particle 
		particles[i].density = particleMass;
		for(int j=i+1; j<particleCount; j++)
		{
			applyDensity( particles[i], particles[j] );
		}

		applySurfaceDensity( particles[i] );

		particles[i].pressure = fluidConstantK * ( particles[i].density - restDensity );
	}
}

void SPHSystem2d::animate( float dt )
{
	if(!particleCount) return;

	for(int i=0; i<particleCount; i++)
	{
		particles[i].reset();
	}
	glm::vec2 rvec;
	
	// Calculating densities and pressures for all particles:
	//  - grid walk
	gridDensityUpdate();
	//  - non grid walk
	// densityUpdate();
	
	// calculating pressure and viscosity forces
	for(int i=0; i<particleCount; i++)
	{		
		// visit all neighbours
		for( size_t j=0, jLen = particles[i].neighbours.size(); j<jLen; j++)
		{
			applyForces( particles[i], *(particles[i].neighbours[j]) );
		}
		
		applySurfaceForces( particles[i] );		
	}

	// TODO: collisions and user interaction
	
	glm::vec2 acceleration;
	glm::vec2 oldPosition;
	glm::vec2 moveVector;

	
	clearGrid( );	
	for(int i=0; i<particleCount; i++)
	{
		SPHParticle2d& particle = particles[i];
		float colorGradientLenSq = glm::length2( particle.colorGradient );
		
		if( colorGradientLenSq >= colorFieldTreshold )
		{
			particle.force += particle.colorGradient*(-surfaceTension*particle.colorLaplacian/sqrt(colorGradientLenSq));
		}

		//particle.force -= particle.velocity*particleMass*0.1;

		// leapfrog
		acceleration = particle.force / particle.density;
		if(useGravity) acceleration += gravityAcc; 

		oldPosition = particle.position;	
		/* particle.velocity * dt or? powf(0.9,dt) */
		particle.position += particle.velocity * dt + particle.oldAcceleration * ( 0.5f*dt*dt );

		/* particle.velocity *? powf(0.9,dt) */
		particle.velocity = particle.velocity + (acceleration + particle.oldAcceleration) * ( 0.5f*dt );
		particle.oldAcceleration = acceleration;
				
		// enforce surfaces
		for (size_t surf = 0, surfLen = surfaces.size(); surf < surfLen; surf++)
		{
			rvec = surfaces[surf]->directionTo( particle );
			surfaces[surf]->enforceInteractor( particle, rvec );			
		}		

		//positions[i] = particle.position;

		putParticleIntoGrid( i );
	}	
}

void SPHSystem2d::draw( MarchingSquaresBase* ms )
{
	unitRadius = sqrt(particleMass / (restDensity*PI));
	float r;
	for(int i=0; i<particleCount; i++)
	{
		r = particles[i].density*10*unitRadius;
		if( r>smoothingLength ) r = smoothingLength;
		ms->putCircle( particles[i].position.x/0.3f, particles[i].position.y/0.3f, r, true );
	}
}

void SPHSystem2d::drawPoints()
{
	glColor3f( 1,1,1 );
	glBegin( GL_POINTS );
	for(int i=0; i<particleCount; i++)
		glVertex2f( particles[i].position.x, particles[i].position.y );
	glEnd();

	glColor3f( 0.3f, 0.9f, 0.3f );
	glBegin( GL_LINES );
	for( int i=1; i<gridWidth; i++)
	{
		glVertex2f( i*smoothingLength, 0 );
		glVertex2f( i*smoothingLength, dHeight );
	}
	glEnd();

	glBegin( GL_LINES );
	for( int i=1; i<gridHeight; i++)
	{
		glVertex2f( 0, i*smoothingLength );
		glVertex2f( dWidth, i*smoothingLength );
	}
	glEnd();

	for (size_t surf = 0, surfLen = surfaces.size(); surf < surfLen; surf++)
	{
		surfaces[surf]->draw();
	}
}

void SPHSystem2d::setUseGravity( bool value )
{
	useGravity = value;
}

bool SPHSystem2d::usesGravity()
{
	return useGravity;
}

int SPHSystem2d::getParticleCount()
{
	return particleCount;
}

void SPHSystem2d::clearAllParticles()
{
	particles.clear();
	clearGrid();
	particleCount = 0;
}

float SPHSystem2d::getRestDensity( )
{
	return restDensity;
}

void SPHSystem2d::setRestDensity( float density )
{
	restDensity = contain<float>( density, 0.0001f, 100 );
	cout << "New density: " << restDensity << endl;
}
	
float SPHSystem2d::getK( )
{
	return fluidConstantK;
}

void SPHSystem2d::setK( float k )
{
	fluidConstantK =  contain<float>( k, 0.00001f, 1000 );
	cout << "New constant K: " << fluidConstantK << endl;
}

float SPHSystem2d::getViscosity( )
{
	return viscosityConstant;
}

void SPHSystem2d::setViscosity( float viscosity )
{
	viscosityConstant = contain<float>( viscosity, 0.0000001f, 10 );
	cout << "New viscosity: " << viscosityConstant << endl;
}

float SPHSystem2d::getSmoothingLength( )
{
	return smoothingLength;
}

void SPHSystem2d::setSmoothingLength( float smLen )
{
	smoothingLength = contain<float>(smLen, 0.1f, 10);
	cout << "New smoothing length: " << smoothingLength << endl;
	pressureKernel->adjustSmoothingLength( smoothingLength );
	viscousKernel->adjustSmoothingLength( smoothingLength );
	kernel->adjustSmoothingLength( smoothingLength );
	createGrid();
}

void SPHSystem2d::paramOutput()
{
	cout << "density: " << restDensity << endl;
	cout << "constant K: " << fluidConstantK << endl;
	cout << "viscosity: " << viscosityConstant << endl;
	cout << "smoothing length: " << smoothingLength << endl;
}