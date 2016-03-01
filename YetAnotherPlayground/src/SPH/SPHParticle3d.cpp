
#include "SPHParticle3d.h"

void SPHParticle3d::reset()
{
	density = 0;
	pressure = 0;
	force = vec3f( 0, 0, 0);

	colorGradient = vec3f( 0, 0, 0 );
	colorLaplacian = 0;
}

void SPHParticleNeighbour3d::reset()
{
	SPHParticle3d::reset();
	neighbours.clear();
}
