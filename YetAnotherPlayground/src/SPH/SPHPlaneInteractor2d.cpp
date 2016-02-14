
#include "SPHPlaneInteractor2d.h"
#include "SPHParticle2d.h"
#include <glm\geometric.hpp>
#include <glm\gtx\norm.hpp>
#include <GL\glew.h>

SPHPlaneInteractor2d::SPHPlaneInteractor2d( vec2f start, vec2f dir, vec2f upDir, float distance )
{
	this->start = start;
	direction = glm::normalize( dir );
	up = glm::normalize( upDir );
	this->distance = distance;
	distanceSquared = distance*distance;
}

SPHPlaneInteractor2d::SPHPlaneInteractor2d( vec2f start, vec2f dir, float distance )
{
	vec2f upDir1 = vec2f( dir.x, -dir.y );
	vec2f upDir2 = vec2f( -dir.x, dir.y );
	vec2f upDir;
	if( glm::dot( upDir1, vec2f(0,1) ) > glm::dot( upDir2, vec2f( 0,1 ) ) )
	{
		upDir = upDir1;
	}else
	{
		upDir = upDir2;
	}

	this->start = start;
	direction = glm::normalize( dir );
	up = glm::normalize( upDir );
	this->distance = distance;
	distanceSquared = distance*distance;
}

void SPHPlaneInteractor2d::applyDensity( SPHParticle2d& other, vec2f rvec )
{
	
}

void SPHPlaneInteractor2d::applyForce( SPHParticle2d& other, vec2f rvec )
{

}

void SPHPlaneInteractor2d::enforceInteractor( SPHParticle2d& other, vec2f rvec )
{	
	if( glm::dot( rvec, up ) < 0 )
	{
		other.position -= rvec;
		rvec = vec2f( 0,0 );
	}
	
	float r = glm::length2( rvec );
	if( r < distanceSquared )
	{
		// offset the particle from the plane
		other.position += up*(distance-sqrtf(r));
		
		// if the particle is moving in the direction oposite of the plane do nothing
		if( glm::dot( other.velocity, up ) > 0 ) return;

		// else, change the direction of the velocity vector (mirror it around the plane axis)
		vec2f axis = direction;
		vec2f v = other.velocity;//.normalized();
		float cosine = glm::dot( axis, v ); 
		if( cosine < 0 )
		{
			axis = -direction;
			cosine = -cosine;
		}
		vec2f newVelocity = axis *( 2*cosine ) - v;
		other.velocity = newVelocity*0.8f;// * other.velocity.norm();	
		other.force -= newVelocity*other.mass;
	}
}

vec2f SPHPlaneInteractor2d::directionTo( SPHParticle2d& other )
{
	vec2f toStart = other.position - start;		
	float t = glm::dot( toStart, direction );
	vec2f projected = start + direction * t;

	return other.position - projected;
}

void SPHPlaneInteractor2d::draw()
{
	vec2f ns, ne;
	ns = start + direction  *0.5f;
	ne = ns + up;
	
	glBegin( GL_LINES );
		glColor3f( 1,0,0 );
		glVertex2f( start.x, start.y );
		glVertex2f( ns.x + direction.x*0.5, ns.y+direction.y*0.5 );

		glColor3f( 0,1,0 );
		glVertex2f( ns.x, ns.y );
		glVertex2f( ne.x, ne.y );

	glEnd();
}