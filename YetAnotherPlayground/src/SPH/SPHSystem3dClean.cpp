
#include "SPHSystem3dClean.h"
#include "SmoothingKernels.h"
#include "MarchingCubes.h"
#include "MarchingCubesShaded.h"
#include "SPHInteractor3d.h"
#include "SPHInteractor3dFactory.h"
#include "MappedData.h"
#include "DataLine.h"
#include "PointDataVisualiser.h"
#include <iostream>
#include <math.h>
#include <glm\common.hpp>
#include <glm\geometric.hpp>
#include <glm\gtx\norm.hpp>

using namespace std;

// Creates a SPH System 3d from base parameters. Default kernels are set and 
// no bounding surfaces are defined, to do so, appropriate methods should be called.
SPHSystem3dClean::SPHSystem3dClean( float w, float h, float d, float density, float constantK, float constantMi, 
							float cfTreshold, float surfTension,  float mass, float smLen )
{
	particles = vector<SPHParticleNeighbour3d>();
	particleCount = 0;
	
	dWidth = w;
	dHeight = h;
	dDepth = d;

	restDensity = density;
	fluidConstantK = constantK;
	viscosityConstant = constantMi;
	colorFieldTreshold = 0.075f;
	colorFieldTreshold *= cfTreshold;
	surfaceTension = surfTension;	
	particleMass = mass;
	unitRadius = sqrt( particleMass / (restDensity*PI) );

	useGravity = true;
	gravityAcc =  vec3f( 0, 0, -9.81 );	

	smoothingLength = smLen;
	kernel = KernelBuilder::getKernel( "KernelPoly6", smoothingLength );
	pressureKernel = KernelBuilder::getKernel( "KernelPoly6", smoothingLength );
	viscousKernel = KernelBuilder::getKernel( "KernelPoly6", smoothingLength );
	
	surfaces = vector<SPHInteractor3d*>();
}

// Creates a SPH System 3d from a mapped data file. The file must contain the following
// groups and fields:
//  - grid: width (float), height (float), surfaces (surface group names)
//  - fluid: density, k, viscosity, colorFieldTreshold, surfaceTension, unitMass (all floats), gravity (two floats)
//  - kernel: smoothingLength (float), base (string), pressure (string), viscous (string)
//  - additional groups describing bounding surfaces provided by SPHInteractor3dFactory
SPHSystem3dClean::SPHSystem3dClean( const char* file )
{
	MappedData map( file );
	particles = vector<SPHParticleNeighbour3d>();
	particleCount = 0;

	surfaces = vector<SPHInteractor3d*>();
	dWidth = map.getData( "grid", "width" ).get<float>();
	dHeight = map.getData( "grid", "height" ).get<float>();
	dDepth = map.getData( "grid", "depth" ).get<float>();
	vector<string> surfaceNames = map.getData( "grid", "surfaces" ).getVector<string>();
	for(size_t i=0, iLen = surfaceNames.size(); i<iLen; i++ )
	{
		surfaces.push_back( SPHInteractor3dFactory::getInteractor( surfaceNames[i], &map ) );
	}

	restDensity = map.getData( "fluid", "density" ).get<float>();
	fluidConstantK = map.getData( "fluid", "k" ).get<float>();
	viscosityConstant = map.getData( "fluid", "viscosity" ).get<float>();
	colorFieldTreshold = map.getData( "fluid", "colorFieldTreshold" ).get<float>();
	colorFieldTreshold *= colorFieldTreshold;
	surfaceTension = map.getData( "fluid", "surfaceTension" ).get<float>();
	particleMass = map.getData( "fluid", "unitMass" ).get<float>();
	gravityAcc =  map.getData( "fluid", "gravity" ).getVec3f() * particleMass;

	smoothingLength = map.getData( "kernel", "smoothingLength" ).get<float>();
	kernel = KernelBuilder::getKernel( map.getData("kernel", "base").getStringData(), smoothingLength );
	pressureKernel = KernelBuilder::getKernel( map.getData("kernel", "pressure").getStringData(), smoothingLength );
	viscousKernel =  KernelBuilder::getKernel( map.getData("kernel", "viscous" ).getStringData(), smoothingLength );
	
	unitRadius = sqrt( particleMass / (restDensity*PI) );
	useGravity = true;	
}


SPHSystem3dClean::~SPHSystem3dClean()
{
	particles.clear();	
	if( kernel ) delete kernel;
	if( pressureKernel ) delete pressureKernel;
	if( viscousKernel ) delete viscousKernel;
}

void SPHSystem3dClean::setKernel( SPHKernelUse kernelUse, KernelType type )
{
	iKernel** old;
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
	if( (*old) ){ 
		delete (*old);
	}
	(*old) =  KernelBuilder::getKernel( type, smoothingLength );
}

void SPHSystem3dClean::addParticle( vec3f position, vec3f velocity )
{
	position = glm::clamp( position, vec3f(0,0,0), vec3f( dWidth, dHeight, dDepth ) );
	particles.push_back(SPHParticleNeighbour3d( position, velocity, particleMass, restDensity ) );
	particleCount++;
}

void SPHSystem3dClean::addSurface( SPHInteractor3d* surface )
{
	surfaces.push_back( surface );
}

void SPHSystem3dClean::toggleSurface(int index)
{
	if (index > -1 && index < (int)surfaces.size())
	{
		surfaces[index]->toggle();
	}
}


void SPHSystem3dClean::applyDensity(SPHParticleNeighbour3d& first, SPHParticleNeighbour3d& second )
{
	vec3f rvec = first.position - second.position;
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
void SPHSystem3dClean::applyForces( SPHParticle3d& first, SPHParticle3d& second )
{
	vec3f rvec = (first.position - second.position);
	float r = glm::length( rvec );
	
	if( r <= 0.000001 ) 
	{
		rvec = vec3f( 0.707, 0.707, 0 );
		r = 1;
	}
	/*
	vec3f commonPressureInfluence = 
		pressureKernel->gradient( rvec ) * 
		( 
			particleMass * 
			(
				second.pressure + first.pressure 
			) / 2.0
		); /* unified */
	vec3f commonPressureInfluence = 
		pressureKernel->gradient( rvec ) * 
		( 
			particleMass * 
			(	
				second.pressure / ( second.density*second.density ) +
				first.pressure / (first.density*first.density)
			)
		);/* by definition */
		
	// viscosity forces
		
	/*vec3f commonViscousInfluence = (second.velocity - first.velocity) * 
		(viscosityConstant * particleMass * viscousKernel->laplacian( r )); /* unified */
	vec3f commonViscousInfluence = 
		(second.velocity - first.velocity) * 
		(
			viscosityConstant * particleMass * 
			viscousKernel->laplacian( r ) / 
			(second.density*first.density)
		);/* by definition */

	first.force += ( commonPressureInfluence + commonViscousInfluence) / second.density;
	second.force += (-commonPressureInfluence - commonViscousInfluence) / first.density;

	vec3f commonColorGradient = kernel->gradient( rvec ) * particleMass;
	first.colorGradient += commonColorGradient / second.density;
	second.colorGradient += commonColorGradient / first.density;

	float commonColorLaplacian = kernel->laplacian( r ) * particleMass;
	first.colorLaplacian += commonColorLaplacian / second.density;
	second.colorLaplacian += commonColorLaplacian / first.density;
		
}

void SPHSystem3dClean::applySurfaceDensity( SPHParticle3d& particle )
{
	vec3f rvec;
	float r;
	for( size_t surf = 0, surfLen = surfaces.size(); surf < surfLen; surf++ )
	{
		rvec = surfaces[surf]->directionTo( particle );
		r = glm::length( rvec );
		if( r < smoothingLength )//smoothingLength )
		{
			particle.density += particleMass * kernel->base(r);
		}
	}
}

void SPHSystem3dClean::applySurfaceForces( SPHParticle3d& particle )
{
	vec3f rvec;
	float r;
	for (size_t surf = 0, surfLen = surfaces.size(); surf < surfLen; surf++)
	{
		rvec = surfaces[surf]->directionTo( particle );
		r = glm::length( rvec );
		if( r < smoothingLength )//smoothingLength )
		{
			// pressure
			particle.force -= pressureKernel->gradient( rvec ) * particleMass * particle.pressure / particle.density;
			// viscosity
			particle.force += ( -particle.velocity ) * (viscousKernel->laplacian( r ) * viscosityConstant * particleMass / particle.density);
		}
	}
}

void SPHSystem3dClean::animate( float dt )
{
	if(!particleCount) return;

	for(int i=0; i<particleCount; i++)
	{
		particles[i].reset();
	}
	
	// Calculating densities and pressures for all particles
	// Non grid, optimization is the calculation of neighbourhood lists
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

	// calculating pressure and viscosity forces
	for(int i=0; i<particleCount; i++)
	{		
		// visit all neighbours
		for (size_t j = 0, jLen = particles[i].neighbours.size(); j<jLen; j++)
		{
			applyForces( particles[i], *(particles[i].neighbours[j]) );
		}
		
		applySurfaceForces( particles[i] );		
	}

	// TODO: collisions and user interaction
	
	vec3f acceleration;
	vec3f oldPosition;
	vec3f moveVector;

	for(int i=0; i<particleCount; i++)
	{
		SPHParticle3d& particle = particles[i];
		float colorGradientLenSq = glm::length2( particle.colorGradient );
		
		if( colorGradientLenSq >= colorFieldTreshold )
		{
			particle.force += particle.colorGradient*(-surfaceTension*particle.colorLaplacian/sqrt(colorGradientLenSq));
		}

		// Drag force
		particle.force -= particle.velocity*particleMass*0.1f;

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
		vec3f rvec;
		float smSquared = smoothingLength*smoothingLength;
		for (size_t surf = 0, surfLen = surfaces.size(); surf < surfLen; surf++)
		{
			rvec = surfaces[surf]->directionTo( particle );
			if( glm::length2( rvec ) < smSquared)
			{
				surfaces[surf]->enforceInteractor( particle, rvec );			
			}
		}	
	//	particle.position.containWithin( vec3f( -10, -10, -10 ), vec3f( 10, 10 ,10 ) );
	}	
}

void SPHSystem3dClean::draw( MarchingCubes* ms )
{
	unitRadius = sqrt(particleMass / (restDensity*PI));
	float r;
	for(int i=0; i<particleCount; i++)
	{
		r = particles[i].density*10*unitRadius;
		if( r>smoothingLength ) r = smoothingLength;
		ms->putSphere( particles[i].position.x/0.4f, particles[i].position.y/0.4f, particles[i].position.z/0.4f, r );
	}
}

void SPHSystem3dClean::draw(MarchingCubesShaded* ms)
{
	unitRadius = sqrt(particleMass / (restDensity*PI));
	float r;
	for (int i = 0; i<particleCount; i++)
	{
		//r = particles[i].density*10*unitRadius;
		//r = particles[i].volume;
		r = unitRadius;
		if (r>smoothingLength) r = smoothingLength;
		ms->putSphere(particles[i].position.x, particles[i].position.y, particles[i].position.z, r);
	}
}

void SPHSystem3dClean::draw( PointDataVisualiser* pdv )
{
	unitRadius = sqrt(particleMass / (restDensity*PI));
	pdv->setPointSize( unitRadius );
	pdv->clearBuffer();
	for(int i=0; i<particleCount; i++)
	{
		pdv->pushPoint( particles[i].position );
	}
}

void SPHSystem3dClean::drawPoints()
{
	glColor3f( 1,1,1 );
	glBegin( GL_POINTS );
	for(int i=0; i<particleCount; i++)
		glVertex3f( particles[i].position.x, particles[i].position.y, particles[i].position.z );
	glEnd();
}

void SPHSystem3dClean::setUseGravity( bool value )
{
	useGravity = value;
}

bool SPHSystem3dClean::usesGravity()
{
	return useGravity;
}

int SPHSystem3dClean::getParticleCount()
{
	return particleCount;
}

void SPHSystem3dClean::clearAllParticles()
{
	particles.clear();
	particleCount = 0;
}

float SPHSystem3dClean::getRestDensity( )
{
	return restDensity;
}

void SPHSystem3dClean::setRestDensity( float density )
{
	restDensity = contain<float>( density, 0.0001f, 100 );
	cout << "New density: " << restDensity << endl;
}
	
float SPHSystem3dClean::getK( )
{
	return fluidConstantK;
}

void SPHSystem3dClean::setK( float k )
{
	fluidConstantK =  contain<float>( k, 0.00001f, 1000 );
	cout << "New constant K: " << fluidConstantK << endl;
}

float SPHSystem3dClean::getViscosity( )
{
	return viscosityConstant;
}

void SPHSystem3dClean::setViscosity( float viscosity )
{
	viscosityConstant = contain<float>( viscosity, 0.0000001f, 10 );
	cout << "New viscosity: " << viscosityConstant << endl;
}

float SPHSystem3dClean::getSmoothingLength( )
{
	return smoothingLength;
}

void SPHSystem3dClean::setSmoothingLength( float smLen )
{
	smoothingLength = contain<float>(smLen, 0.1f, 10);
	cout << "New smoothing length: " << smoothingLength << endl;
	pressureKernel->adjustSmoothingLength( smoothingLength );
	viscousKernel->adjustSmoothingLength( smoothingLength );
	kernel->adjustSmoothingLength( smoothingLength );	
}

float SPHSystem3dClean::getColorFieldTreshold( )
{
	return colorFieldTreshold;
}

void SPHSystem3dClean::setColorFieldTreshold( float cfTreshold )
{
	colorFieldTreshold = contain<float>(cfTreshold, 0.005f, 1);
	cout << "New CFT: " << colorFieldTreshold << endl;
}

float SPHSystem3dClean::getSurfaceTension( )
{
	return surfaceTension;
}

void SPHSystem3dClean::setSurfaceTension( float sTension )
{
	surfaceTension = contain<float>(sTension, 0.005f, 1);
	cout << "New surface tension: " << surfaceTension << endl;
}