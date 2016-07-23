
#include "SPHPlaneInteractor3d.h"
#include "SPHParticle3d.h"
#include <glm\gtx\norm.hpp>
#include <glm\geometric.hpp>
#include <GL\glew.h>

SPHPlaneInteractor3d::SPHPlaneInteractor3d( glm::vec3 start, glm::vec3 upNormal, float distance )
{
	this->start = start;	
	up = glm::normalize( upNormal );
	this->distance = distance;
	distanceSquared = distance*distance;
}

void SPHPlaneInteractor3d::applyDensity( SPHParticle3d& other, glm::vec3 rvec )
{
	
}

void SPHPlaneInteractor3d::applyForce( SPHParticle3d& other, glm::vec3 rvec )
{

}

void SPHPlaneInteractor3d::enforceInteractor( SPHParticle3d& other, glm::vec3 rvec )
{	
	if(!turnedOn) return;
	rvec *= -1;
	if( glm::dot( rvec, up ) < 0 )
	{
		other.position -= rvec;
		rvec = glm::vec3( 0,0,0 );
	}
	
	float r = glm::length2( rvec );
	if( r < distanceSquared )
	{
		
		// if the particle is moving in the direction oposite of the plane do nothing
		if( glm::dot( other.velocity, up ) > 0 ) return;
		
		// offset the particle from the plane
		other.position += up*(distance-sqrtf(r));
		
		// else, change the direction of the velocity vector (mirror it around the plane axis)
		glm::vec3 axis = up;
		glm::vec3 v = other.velocity;//.normalized();
		float cosine = glm::dot( up, v ); 
	/*	if( cosine < 0 )
		{
			axis = -up;
			cosine = -cosine;
		}/**/
		glm::vec3 newVelocity = v - up * ( 1.8f * cosine );
		other.velocity = newVelocity;// * other.velocity.norm();	
		//other.force += up*other.mass;//*other.velocity.norm();
	}
}

glm::vec3 SPHPlaneInteractor3d::directionTo( SPHParticle3d& other )
{/*
	glm::vec3 toStart = other.position - start;		
	float t = toStart.dotProduct( up );
	glm::vec3 projected = start + direction * t;

	return other.position - projected;*/

	float D = - glm::dot( up, other.position-start );
	return up*D;
}

void SPHPlaneInteractor3d::draw()
{
	// TODO: draw interactors
}