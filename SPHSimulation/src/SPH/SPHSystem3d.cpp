
#include "SPHSystem3d.h"
#include "MarchingCubes.h"
#include "MarchingCubesBasic.h"
#include "SPHInteractor3d.h"
#include "SPHInteractor3dFactory.h"
#include "PointDataVisualiser.h"
#include "MappedData.h"
#include "MarchingCubesShaded.h"
#include <iostream>
#include <math.h>

using namespace std;

// Creates a SPH System 3d from base parameters. Default kernels are set and 
// no bounding surfaces are defined, to do so, appropriate methods should be called.
SPHSystem3d::SPHSystem3d( float w, float h, float d, float density, float constantK, float constantMi, 
							float cfTreshold, float surfTension,  float mass, float smLen ):
	particleCount(0),
	dWidth(w), dHeight(h), dDepth(d),
	gridWidth(-1), gridHeight(-1),
	restDensity(density), fluidConstantK(constantK), viscosityConstant(constantMi),
	colorFieldTreshold(0.075f * cfTreshold), surfaceTension(surfTension), particleMass(mass),
	unitRadius(mass/(density*PI)), useGravity(true), gravityAcc(0.0f, 0.0f, -9.81f)
{
	adjustSmoothingLength( smLen );
}

// Creates a SPH System 3d from a mapped data file. The file must contain the following
// groups and fields:
//  - grid: width (float), height (float), surfaces (surface group names)
//  - fluid: density, k, viscosity, colorFieldTreshold, surfaceTension, unitMass (all floats), gravity (two floats)
//  - kernel: smoothingLength (float), base (string), pressure (string), viscous (string)
//  - additional groups describing bounding surfaces provided by SPHInteractor3dFactory
SPHSystem3d::SPHSystem3d( const char* file ):
	particleCount(0),
	useGravity(true),
	gridWidth(-1), gridHeight(-1), gridDepth(-1)
{
	MappedData map( file );

	dWidth = map.getData( "grid", "width" ).get<float>();
	dHeight = map.getData( "grid", "height" ).get<float>();
	dDepth = map.getData( "grid", "depth" ).get<float>();
	vector<string> surfaceNames = map.getData( "grid", "surfaces" ).getVector<string>();
	for ( string sName : surfaceNames )
	{
		surfaces.push_back( SPHInteractor3dFactory::getInteractor( sName, &map ) );
	}

	restDensity = map.getData( "fluid", "density" ).get<float>();
	fluidConstantK = map.getData( "fluid", "k" ).get<float>();
	viscosityConstant = map.getData( "fluid", "viscosity" ).get<float>();
	colorFieldTreshold = map.getData( "fluid", "colorFieldTreshold" ).get<float>();
	//colorFieldTreshold *= colorFieldTreshold;
	surfaceTension = map.getData( "fluid", "surfaceTension" ).get<float>();
	particleMass = map.getData( "fluid", "unitMass" ).get<float>();
	gravityAcc =  map.getData( "fluid", "gravity" ).getVec3();
		
	unitRadius = sqrt( particleMass / (restDensity*PI) );
	
	adjustSmoothingLength( map.getData( "kernel", "smoothingLength" ).get<float>() );
}


SPHSystem3d::~SPHSystem3d()
{
	// No special cleanup needed
	/*particles.clear();	
	grid.clear();	
	surfaces.clear();
	pairs.clear();*/
}

void SPHSystem3d::clearGrid()
{
	for(int i=0; i< gridHeight*gridWidth*gridDepth; i++)
	{
		grid[i].clear();
	}
}

void SPHSystem3d::createGrid()
{
	int newGridHeight = (int)ceil( dHeight / smoothingLength );
	int newGridWidth = (int)ceil( dWidth / smoothingLength );
	int newGridDepth = (int)ceil( dDepth / smoothingLength );
	// If the grid dimensions change, rebuild the grid vectors
	if( gridWidth != newGridWidth || gridHeight!= newGridHeight || gridDepth!=newGridDepth)
	{
		/*gridWidth = newGridWidth;
		gridHeight = newGridHeight;
		gridDepth = newGridDepth;/**/
		gridWidth = 3;
		gridHeight = 3;
		gridDepth = 3;/**/
		grid.clear();
		for(int i=0; i< gridHeight*gridWidth*gridDepth; i++)
		{
			grid.push_back( vector< int >() );
		}
	}else	// else just clear the existing grid vectors
	{
		clearGrid();
	}
	fillGrid();	
		
 /* Build index offsets, C current cell, (A,R,F) cell to visit, X ignored cell
	dx			-1				 0				 +1 
	--------------------------------------------------------------------------
	 dz\dy	-1   0  +1		-1   0  +1		-1   0  +1	  dy/dz
	 +1		 X | X | X		 A | A | A		 F | F | F		+1
			-----------		-----------		-----------
	  0		 X | X | X		 X | C | R		 F | F | F		 0
			-----------		-----------		-----------
	 -1		 X | X | X		 X | X | X		 F | F | F		-1

	*/

	int gridOffIndex=0;
	// F cells
	for( int yf = -1; yf<2; yf++)
	{	
		for( int zf = -1; zf<2; zf++)
		{
			gridOffsets[gridOffIndex]= (zf*gridHeight + yf)*gridWidth+1;					
			gridOffIndex++;
		}
	}

	// A cells
	for( int ya =-1; ya<2; ya++)
	{	
		gridOffsets[gridOffIndex]= (1*gridHeight + ya)*gridWidth;
		gridOffIndex++;
	}
	// R cell
	gridOffsets[gridOffIndex] = 1*gridWidth;

}

void SPHSystem3d::fillGrid( )
{
	for( int i = 0; i<particleCount; i++ )
	{
		putParticleIntoGrid( i );
	}
}

void SPHSystem3d::putParticleIntoGrid( int particleIndex )
{
	// Calculate grid index
	glm::vec3 position = particles[ particleIndex ].position;
	int x = (int) (position.x * gridWidth / dWidth);
	int y = (int) (position.y * gridHeight / dHeight);
	int z = (int) (position.z * gridDepth / dDepth);
	if( x >= gridWidth ){
		x=gridWidth-1;
		particles[ particleIndex ].position.x = dWidth;
	}
	if( x < 0 )
	{
		x = 0;
		particles[ particleIndex ].position.x = 0;
	}
	if( y >= gridHeight )
	{
		y=gridHeight-1;
		particles[ particleIndex ].position.y = dHeight;
	}
	if( y < 0 )
	{
		y = 0;
		particles[ particleIndex ].position.y = 0;
	}
	if( z >= gridDepth )
	{
		z=gridDepth-1;
		particles[ particleIndex ].position.z = dDepth;
	}
	if( z < 0 )
	{
		z = 0;
		particles[ particleIndex ].position.z = 0;
	}

	grid[ (z*gridHeight + y)*gridWidth + x ].push_back( particleIndex );
}

void SPHSystem3d::addParticle( glm::vec3 position, glm::vec3 velocity )
{
	position = glm::clamp( position, glm::vec3(0,0,0), glm::vec3( dWidth, dHeight, dDepth ) );
	// density used to be restDensity, not 0
	particles.push_back( SPHParticle3d( position, velocity, particleMass, 0 ) );
	putParticleIntoGrid( particleCount );
	particleCount++;	
}

void SPHSystem3d::addSurface(std::unique_ptr<SPHInteractor3d>& surface )
{
	surfaces.push_back( std::move(surface) );
}

void SPHSystem3d::toggleSurface( int index )
{
	if( index > -1 && index < (int)surfaces.size() )
	{
		surfaces[index]->toggle();
	}
}

// NOTE: compute only the kernel into density, mass is the same for all particles
// therefore it can be multiplied into density after all density updates
void SPHSystem3d::applyDensity( SPHParticle3d& first, SPHParticle3d& second )
{
	glm::vec3 rvec = first.position - second.position;
	float rSq = glm::length2( rvec );	
	if( rSq < hSquared )
	{
		float additionalDensity = kp6base(rSq);
		first.density += additionalDensity;
		second.density += additionalDensity;
		//first.neighbours.push_back( &second );
		pairs.push_back( SPHPair( first, second, rvec ) );
	}
}

// NOTE: Assume this is called on neighbourhood data. No smoothing check is made.
void SPHSystem3d::applyForces( SPHParticle3d& first, SPHParticle3d& second )
{
	glm::vec3 rvec = (first.position - second.position);
	applyForces( first, second, rvec );		
}

// NOTE: Assume this is called on pair data. No smoothing check is made.
void SPHSystem3d::applyForces( SPHParticle3d& first, SPHParticle3d& second, glm::vec3 rvec )
{
	float r = glm::length( rvec );
	
	if( r <= 0.00173f ) 
	{
		rvec = glm::vec3( 0.001f, 0.001f, 0.001f );
		r = 0.00173f;
	}
	
	glm::vec3 commonPressureInfluence = 
		ksgradient( rvec ) * 
		( 
			//particleMass * 
			(
				second.pressure + first.pressure 
			) / 2.0f
		); /* unified */
	/*glm::vec3 commonPressureInfluence = 
		ksgradient( rvec )*
		( 
			particleMass * 
			(	
				second.pressure / ( second.density*second.density ) +
				first.pressure / (first.density*first.density)
			)
		);/* by definition */
		
	// viscosity forces
		
	glm::vec3 commonViscousInfluence = (second.velocity - first.velocity) * 
		(viscosityConstant/* * particleMass/**/ * kvlaplacian( r )); /* unified */
	/*glm::vec3 commonViscousInfluence = 
		(second.velocity - first.velocity) * 
		(
			viscosityConstant * particleMass * 
			kvlaplacian( r ) /
			(second.density*first.density)
		);/* by definition */

	first.force += ( -commonPressureInfluence + commonViscousInfluence) * second.volume; /// second.density;
	second.force += ( commonPressureInfluence - commonViscousInfluence) * first.volume; /// first.density;
	
	glm::vec3 commonColorGradient = kp6gradient( rvec );// * particleMass;
	first.colorGradient += commonColorGradient * second.volume; /// second.density;
	second.colorGradient += commonColorGradient * first.volume;/// first.density;

	float commonColorLaplacian = kp6laplacian( r*r );// * particleMass;
	first.colorLaplacian += commonColorLaplacian * second.volume; /// second.density;
	second.colorLaplacian += commonColorLaplacian * first.volume;// / first.density;
		
}

// NOTE: compute only the kernel into density, mass is the same for all particles
// therefore it can be multiplied into density after all density updates
void SPHSystem3d::applySurfaceDensity( SPHParticle3d& particle )
{
	glm::vec3 rvec;
	float rSq;
	for( size_t surf = 0, surfLen = surfaces.size(); surf < surfLen; surf++)
	{
		rvec = surfaces[surf]->directionTo( particle );
		rSq = glm::length2( rvec );
		if( rSq < hSquared )
		{
			particle.density += kp6base(rSq);
		}
	}
}

void SPHSystem3d::applySurfaceForces( SPHParticle3d& particle )
{
	glm::vec3 rvec;
	float rSq;
	glm::vec3 oldForce;
	for (size_t surf = 0, surfLen = surfaces.size(); surf < surfLen; surf++)
	{
		rvec = surfaces[surf]->directionTo( particle );
		rSq = glm::length2( rvec );
		if( rSq < hSquared )
		{
			/*if( rSq <= 0.000003f ) 
			{
				if( rSq == 0.0f )
				{
					rvec.set(0.001f, 0.001f, 0.001f);
				}else{
					rvec.normalize();
					rvec *= 0.001732f;
				}
				rSq = 0.000003f;
			}/**/

			oldForce = particle.force;
			if( _isnan(particle.force.x) == 1 ) 	
			{
				cout << "1";
			}
			// pressure
			//particle.force += ksgradient( rvec ) * particleMass * particle.pressure / particle.density;
			float pressure = particle.pressure;
			//float pressure = particle.pressure < 0 ? -particle.pressure : particle.pressure;
			particle.force += (ksgradient( rvec ) * pressure * particle.volume)*0.5f;
			if( _isnan(particle.force.x) == 1 ) 	
			{
				cout << "2";
			}
			// viscosity
			//particle.force -= ( particle.velocity ) * (kvlaplacian( sqrtf(rSq) ) * viscosityConstant * particleMass / particle.density);
			particle.force += ( particle.velocity ) * (kvlaplacian( sqrtf(rSq) ) * viscosityConstant * particle.volume);
			if( _isnan(particle.force.x) == 1 ) 	
			{
				cout << "3";
				particle.force = oldForce;
			}
		}
	}
}

// ALMOST DONE!!
void SPHSystem3d::gridDensityUpdate( )
{	
	vector<int>* neighbourCell;
	vector<int>* thisCell;
	int cellIndex;
	int particlesInCell;
	
	for( int x=0; x<gridWidth; x++)
	{
	for( int y=0; y<gridHeight; y++)
	{	
	for(int z=0; z<gridDepth; z++)
	{
		cellIndex = (z*gridHeight + y)*gridWidth+x;
		thisCell = &grid[cellIndex];
		particlesInCell = thisCell->size();
		if( particlesInCell < 1 ) continue;
		
		int mask = 0x1fff;
		if( x+1 == gridWidth )
		{
			mask &= 0xf;
		}
		if( y == 0 )
		{
			mask &= 0x3f7;
		}
		if( y+1 == gridHeight )
		{
			mask &= 0x1f8c;
		}
		if( z == 0 )
		{
			mask &= 0xdbf;
		}
		if( z+1 == gridDepth )
		{
			mask &= 0x1b61;
		}		
				
		for(int particleInd=0; particleInd < particlesInCell; particleInd++)	// particle index
		{				
			SPHParticle3d& particle = particles[ (*thisCell)[particleInd] ];
			//particle.density += particleMass;

			for( int otherInd=particleInd+1; otherInd < particlesInCell; otherInd++ )
			{
				applyDensity( particle, particles[ (*thisCell)[otherInd] ] );
			}
			
			for(int gi=12; gi>=0; gi--)
			{
				if( mask & 0x1 ) // last bit is set
				{
					neighbourCell = &grid[ gridOffsets[gi]+cellIndex ];
					for( size_t k=0, kLen = neighbourCell->size(); k<kLen; k++ )
					{
						applyDensity( particle, particles[ (*neighbourCell)[k] ] );
					}
				}
				mask >>= 1;
			}
			
			applySurfaceDensity( particle );

			particle.density *= particleMass;
			particle.pressure = fluidConstantK * (particle.density - restDensity );
		}
	}
	}
	}
}

void SPHSystem3d::densityUpdate()
{	
	for(int i=0; i<particleCount; i++)
	{		
		SPHParticle3d& particle = particles[i];
		// particle - particle 
		//particles[i].density = particleMass;	// this creates problems, without it it is even worse
		particle.density += 1;//*restDensity;
		for(int j=i+1; j<particleCount; j++)
		{
			applyDensity( particle, particles[j] );
		}

		//applySurfaceDensity( particles[i] );
		particle.volume = 1.0f/particle.density;
		particle.density *= particleMass;
		particle.pressure = fluidConstantK * ( particle.density - restDensity )/restDensity;
	}
}

void SPHSystem3d::animate( float dt )
{
	if(!particleCount) return;
	pairs.clear();

	for(int i=0; i<particleCount; i++)
	{
		particles[i].reset();
	}
	glm::vec3 rvec;

	bool useGrid = false;
	
	if(useGrid)
		gridDensityUpdate(); 
	else
		densityUpdate();

	// Visit pairs
	for(size_t i=0, iLen = pairs.size(); i<iLen; i++)
	{
		applyForces( pairs[i].first, pairs[i].second, pairs[i].rvec );
	}/**/

	// calculating pressure and viscosity forces
	/*int neighboursCount;
	for(int i=0; i<particleCount; i++)
	{		
		neighboursCount = particles[i].neighbours.size();
		// visit all neighbours
		for( int j=0; j<neighboursCount; j++)
		{
			applyForces( particles[i], *(particles[i].neighbours[j]) );
		}
		
		applySurfaceForces( particles[i] );		
	}/**/

	// TODO: collisions and user interaction
	
	glm::vec3 acceleration;
	glm::vec3 oldPosition;
	glm::vec3 newVelocity;

	if(useGrid)
		clearGrid( );	
	bool wasOK;
	float cftsq = colorFieldTreshold*colorFieldTreshold;
	for(int i=0; i<particleCount; i++)
	{
		SPHParticle3d& particle = particles[i];
		// true if the number was ok
		// false if it is not, this is not interesting
		wasOK = _isnan(particle.force.x) == 0;
		if( !wasOK )
		{
			cout << "0";
		}

		// this is not done since pairs mechanism is used
		applySurfaceForces( particle );	

		if(wasOK != ( _isnan(particle.force.x) == 0 ) )	
		{
			cout << "1";
			wasOK = false;
		}

		// if number changed status, from beeing ok to not beeing ok
		// or if it became ok after beeing false, which will not happen
		/*if(wasOK != ( _isnan(particle.position.x) == 0 ) )	
		{
			cout << "0";
			wasOK = false;
		}*/
		// enforce surfaces - old
		
		
		float colorGradientLenSq = glm::length2( particle.colorGradient );		
		if( colorGradientLenSq > cftsq )	// before was >=
		{
			particle.force += particle.colorGradient*(-surfaceTension*particle.colorLaplacian/sqrt(colorGradientLenSq));
			// before -surfaceTension
		}
		/**/
		if(wasOK != ( _isnan(particle.force.x) == 0 ) )	
		{
			cout << "3";
			wasOK = false;
		}

		//particle.force -= particle.velocity*particleMass*0.1;

		// leapfrog
		acceleration = particle.force ;// particle.density;		
		if(useGravity) acceleration += gravityAcc; 

		if(wasOK != ( _isnan(particle.force.x) == 0 ) )	
		{
			cout << "4";
			wasOK = false;
		}

		oldPosition = particle.position;	
		newVelocity = particle.velocity * powf(0.9f,dt);
		particle.position += newVelocity * /**/ dt + particle.oldAcceleration * ( 0.5f*dt*dt );

		particle.velocity = newVelocity /**/ + (acceleration + particle.oldAcceleration) * ( 0.5f*dt );
		particle.oldAcceleration = acceleration;

		//particle.position.containWithin( glm::vec3(0,0,0), glm::vec3( dWidth, dHeight, dDepth ));

		if(wasOK != ( _isnan(particle.force.x) == 0 ) )	
		{
			cout << "5";
			wasOK = false;
		}

		for( size_t surf = 0, surfLen = surfaces.size(); surf < surfLen; surf++)
		{
			rvec = surfaces[surf]->directionTo( particle );
			surfaces[surf]->enforceInteractor( particle, rvec );			
		}

		if(wasOK != ( _isnan(particle.force.x) == 0 ) )	
		{
			cout << "2";
			wasOK = false;
		}
						
		
		if(useGrid) putParticleIntoGrid( i );
	}	
}

void SPHSystem3d::draw( MarchingCubes* ms )
{
	unitRadius = sqrt(particleMass / (restDensity*PI));
	float r;
	for(int i=0; i<particleCount; i++)
	{
		r = particles[i].density*10*unitRadius;
		//r = particles[i].volume;
		if( r>smoothingLength ) r = smoothingLength;
		ms->putSphere( particles[i].position.x/0.4f, particles[i].position.y/0.4f, particles[i].position.z/0.4f, r/0.4f );
	}
}

void SPHSystem3d::draw( MarchingCubesBasic* ms )
{
	unitRadius = sqrt(particleMass / (restDensity*PI));
	float r ;
	for(int i=0; i<particleCount; i++)
	{
		//r = particles[i].density*10*unitRadius;
		//r = particles[i].volume;
		r = unitRadius;
		if( r>smoothingLength ) r = smoothingLength;
		ms->putSphere( particles[i].position.x, particles[i].position.y, particles[i].position.z, r );
	}
}

void SPHSystem3d::draw( MarchingCubesShaded* ms )
{
	unitRadius = sqrt(particleMass / (restDensity*PI));
	float r ;
	for(int i=0; i<particleCount; i++)
	{
		//r = particles[i].density*10*unitRadius;
		//r = particles[i].volume;
		r = unitRadius;
		if( r>smoothingLength ) r = smoothingLength;
		ms->putSphere( particles[i].position.x, particles[i].position.y, particles[i].position.z, r );
	}
}


void SPHSystem3d::draw( PointDataVisualiser* pdv )
{
	unitRadius = sqrt(particleMass / (restDensity*PI));
	pdv->setPointSize( unitRadius );
	pdv->clearBuffer();
	for(int i=0; i<particleCount; i++)
	{
		pdv->pushPoint( particles[i].position );
	}
	/*for( int i=0; i<surfaces.size(); i++)
	{
		surfaces[i]->draw();
	}*/
}

void SPHSystem3d::drawPoints()
{
	glColor3f( 1,1,1 );
	glBegin( GL_POINTS );
	for(int i=0; i<particleCount; i++)
		glVertex3f( particles[i].position.x, particles[i].position.y, particles[i].position.z );
	glEnd();
}

void SPHSystem3d::setUseGravity( bool value )
{
	useGravity = value;
}

bool SPHSystem3d::usesGravity()
{
	return useGravity;
}

int SPHSystem3d::getParticleCount()
{
	return particleCount;
}

void SPHSystem3d::clearAllParticles()
{
	particles.clear();
	clearGrid();
	particleCount = 0;
}

float SPHSystem3d::getRestDensity( )
{
	return restDensity;
}

void SPHSystem3d::setRestDensity( float density )
{
	restDensity = contain<float>( density, 0.01f, 3.0f );
	cout << "New density: " << restDensity << endl;
}
	
float SPHSystem3d::getK( )
{
	return fluidConstantK;
}

void SPHSystem3d::setK( float k )
{
	fluidConstantK =  contain<float>( k, 0.05f, 100 );
	cout << "New constant K: " << fluidConstantK << endl;
}

float SPHSystem3d::getViscosity( )
{
	return viscosityConstant;
}

void SPHSystem3d::setViscosity( float viscosity )
{
	viscosityConstant = contain<float>( viscosity, 0.01f, 10 );
	cout << "New viscosity: " << viscosityConstant << endl;
}

float SPHSystem3d::getSmoothingLength( )
{
	return smoothingLength;
}

void SPHSystem3d::setSmoothingLength( float smLen )
{
	adjustSmoothingLength( contain<float>(smLen, 0.1f, 10) );
	cout << "New smoothing length: " << smoothingLength << endl;		
}

float SPHSystem3d::getColorFieldTreshold( )
{
	return colorFieldTreshold;
}

void SPHSystem3d::setColorFieldTreshold( float cfTreshold )
{
	colorFieldTreshold = contain<float>(cfTreshold, 0.005f, 5);
	cout << "New CFT: " << colorFieldTreshold << endl;
}

float SPHSystem3d::getSurfaceTension( )
{
	return surfaceTension;
}

void SPHSystem3d::setSurfaceTension( float sTension )
{
	surfaceTension = contain<float>(sTension, 0.005f, 5);
	cout << "New surface tension: " << surfaceTension << endl;
}

void SPHSystem3d::paramOutput()
{
	cout << "density: " << restDensity << endl;
	cout << "constant K: " << fluidConstantK << endl;
	cout << "viscosity: " << viscosityConstant << endl;
	cout << "smoothing length: " << smoothingLength << endl;
}

void SPHSystem3d::adjustSmoothingLength( float h )
{
	this->smoothingLength = h;	
	kp6baseFactor = 315.0f / (64 * PI * pow( h, 9 ));
	// CHECK
	// this gives a negative gradient graph, in Muller the graph is positive...
	kp6gradientFactor = 6.0f*kp6baseFactor;// -6*315 / (64 * PI * pow( h, 9 ) );
	kp6laplacianFactor = 24.0f*kp6baseFactor;//24*315 / (64 * PI * pow( h, 9 ) );
	// changed sign of gradient

	ksbaseFactor = 15.0f / ( PI * pow( h, 6 ) );
	ksgradientFactor = -3 * ksbaseFactor;// -45.0 / ( PI * pow( h, 6 ) );
	kslaplacianFactor = 6 * ksbaseFactor;
	// not-changed signs of gradient and laplacian

	kvbaseFactor = 15 / ( 2 * PI * pow( h, 3 ) );
	kvgradientFactor = -15 / ( 2 * PI * pow( h, 3 ) );
	kvlaplacianFactor = 45 / ( PI * pow( h, 6 ) );

	hSquared = h*h;
	createGrid();
}
