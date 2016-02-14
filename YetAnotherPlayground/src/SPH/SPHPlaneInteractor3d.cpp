
#include "SPHPlaneInteractor3d.h"
#include "SPHParticle3d.h"
#include <glm\gtx\norm.hpp>
#include <glm\geometric.hpp>
#include <GL\glew.h>

SPHPlaneInteractor3d::SPHPlaneInteractor3d( vec3f start, vec3f upNormal, float distance )
{
	this->start = start;	
	up = glm::normalize( upNormal );
	this->distance = distance;
	distanceSquared = distance*distance;
}

void SPHPlaneInteractor3d::applyDensity( SPHParticle3d& other, vec3f rvec )
{
	
}

void SPHPlaneInteractor3d::applyForce( SPHParticle3d& other, vec3f rvec )
{

}

void SPHPlaneInteractor3d::enforceInteractor( SPHParticle3d& other, vec3f rvec )
{	
	if(!turnedOn) return;
	rvec *= -1;
	if( glm::dot( rvec, up ) < 0 )
	{
		other.position -= rvec;
		rvec = vec3f( 0,0,0 );
	}
	
	float r = glm::length2( rvec );
	if( r < distanceSquared )
	{
		
		// if the particle is moving in the direction oposite of the plane do nothing
		if( glm::dot( other.velocity, up ) > 0 ) return;
		
		// offset the particle from the plane
		other.position += up*(distance-sqrtf(r));
		
		// else, change the direction of the velocity vector (mirror it around the plane axis)
		vec3f axis = up;
		vec3f v = other.velocity;//.normalized();
		float cosine = glm::dot( up, v ); 
	/*	if( cosine < 0 )
		{
			axis = -up;
			cosine = -cosine;
		}/**/
		vec3f newVelocity = v - up * ( 1.8f * cosine );
		other.velocity = newVelocity;// * other.velocity.norm();	
		//other.force += up*other.mass;//*other.velocity.norm();
	}
}

vec3f SPHPlaneInteractor3d::directionTo( SPHParticle3d& other )
{/*
	vec3f toStart = other.position - start;		
	float t = toStart.dotProduct( up );
	vec3f projected = start + direction * t;

	return other.position - projected;*/

	float D = - glm::dot( up, other.position-start );
	return up*D;
}

void SPHPlaneInteractor3d::draw()
{
	/*vec3f ns, ne;
	ns = start + direction*0.5;
	ne = ns + up;*/
	
	glBegin( GL_LINES );
		glColor3f( 1,0,0 );
		glVertex3f( start.x, start.y, start.z );
		glVertex3f( start.x + up.x, start.y+up.y, start.z+up.z);
	glEnd();
}