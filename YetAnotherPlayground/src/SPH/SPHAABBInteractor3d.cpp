
#include "SPHAABBInteractor3d.h"
#include "SPHParticle3d.h"
#include <iostream>
#include <glm\common.hpp>
#include <gl\glew.h>

SPHAABBInteractor3d::SPHAABBInteractor3d( vec3f min, vec3f max, float dampen, float distance ):
	min( min ), max( max), dampening( dampen ), distance( distance ), distanceSquared( distance*distance )
{	
}

void SPHAABBInteractor3d::applyDensity( SPHParticle3d& other, vec3f rvec )
{
	
}

void SPHAABBInteractor3d::applyForce( SPHParticle3d& other, vec3f rvec )
{

}

void SPHAABBInteractor3d::enforceInteractor( SPHParticle3d& other, vec3f rvec )
{	
	
	if( !isWithin( other.position, min, max ) )
	{	// outside - move inside
		other.position = glm::clamp(other.position, min, max);
		//rvec.normalize();
		//rvec *= distance;
		//other.position -= rvec;
		//rvec.changeSign();
		if( rvec.x != 0 )
		{
			other.velocity.x = -other.velocity.x*dampening;
			//other.position.x -= rvec.x;
		}
		if( rvec.y != 0 )
		{
			other.velocity.y = -other.velocity.y*dampening;
			//other.position.y -= rvec.y;
		}
		if( rvec.z != 0 )
		{
			other.velocity.z = -other.velocity.z*dampening;
			//other.position.z -= rvec.z;
		}
	}
	/*
	// inside - reflect speed
	float r = rvec.norm();
	float depth = distance - r;	
	//vec3f dir = rvec.normalized();
	float cosine = rvec.dotProduct( other.velocity );
	if( depth == 0 && cosine >= 0 )
	{		
		if( rvec.x != 0 )
		{
			other.velocity.x = -other.velocity.x*dampening;
			other.position.x -= rvec.x;
		}
		if( rvec.y != 0 )
		{
			other.velocity.y = -other.velocity.y*dampening;
			other.position.y -= rvec.y;
		}
		if( rvec.z != 0 )
		{
			other.velocity.z = -other.velocity.z*dampening;
			other.position.z -= rvec.z;
		}
		/*
		other.position -= rvec*(depth);
		vec3f newVelocity = other.velocity-rvec*2*cosine/(r*r);
		//cosine = abs(cosine);
		//newVelocity *= dir*(1.7);
		if( rvec.maxPart() != 0 ) 
		{				
			newVelocity *= -1;
		}
		other.velocity = newVelocity;		/**/
	//}
}

vec3f SPHAABBInteractor3d::directionTo( SPHParticle3d& other )
{
	vec3f position = other.position;
	vec3f toMin = position - min;
	vec3f toMax = max - position;
	float minMin = minPart(toMin);
	float minMax = minPart(toMax);
	//if(position.x-0.036558483<=0.0000001 && position.y-0.074678488<=0.0000001 )
	//{
	//	int a = 0;
	//}
	if( minMin < minMax )
	{
		if( minMin < distance )
			return minDir(toMin)*(-distance);
		return -minDim(toMin);
	}else{
		if( minMax < distance )
			return minDir(toMax)*distance;
		return minDim(toMax);	// ERROR? missing minus?
	}
}

void SPHAABBInteractor3d::draw()
{	
	glDisable( GL_LIGHTING );
	glBegin( GL_LINES );
		glColor3f( 1,0,0 );
		glVertex3f( min.x, min.y, min.z );
		glVertex3f( max.x, min.y, min.z );

		glVertex3f( min.x, min.y, min.z );
		glVertex3f( min.x, max.y, min.z );

		glVertex3f( min.x, min.y, min.z );
		glVertex3f( min.x, min.y, max.z );

		glVertex3f( max.x, max.y, max.z );
		glVertex3f( min.x, max.y, max.z );

		glVertex3f( max.x, max.y, max.z );
		glVertex3f( max.x, min.y, max.z );

		glVertex3f( max.x, max.y, max.z );
		glVertex3f( max.x, max.y, min.z );

		glVertex3f( max.x, min.y, min.z );
		glVertex3f( max.x, min.y, max.z );

		glVertex3f( max.x, min.y, min.z );
		glVertex3f( max.x, max.y, min.z );

		glVertex3f( min.x, max.y, min.z );
		glVertex3f( min.x, max.y, max.z );

		glVertex3f( min.x, max.y, min.z );
		glVertex3f( max.x, max.y, min.z );

		glVertex3f( min.x, min.y, max.z );
		glVertex3f( min.x, max.y, max.z );

		glVertex3f( min.x, min.y, max.z );
		glVertex3f( max.x, min.y, max.z );

	glEnd();
}