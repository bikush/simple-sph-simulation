
#include "SPHPlaneInteractor2d.h"
#include "SPHParticle2d.h"
#include <glm\geometric.hpp>
#include <glm\gtx\norm.hpp>
#include <GL\glew.h>

SPHPlaneInteractor2d::SPHPlaneInteractor2d( glm::vec2 start, glm::vec2 dir, glm::vec2 upDir, float distance )
{
	this->start = start;
	direction = glm::normalize( dir );
	up = glm::normalize( upDir );
	this->distance = distance;
	distanceSquared = distance*distance;
}

SPHPlaneInteractor2d::SPHPlaneInteractor2d( glm::vec2 start, glm::vec2 dir, float distance )
{
	glm::vec2 upDir1 = glm::vec2( dir.x, -dir.y );
	glm::vec2 upDir2 = glm::vec2( -dir.x, dir.y );
	glm::vec2 upDir;
	if( glm::dot( upDir1, glm::vec2(0,1) ) > glm::dot( upDir2, glm::vec2( 0,1 ) ) )
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

void SPHPlaneInteractor2d::applyDensity( SPHParticle2d& other, glm::vec2 rvec )
{
	
}

void SPHPlaneInteractor2d::applyForce( SPHParticle2d& other, glm::vec2 rvec )
{

}

void SPHPlaneInteractor2d::enforceInteractor( SPHParticle2d& other, glm::vec2 rvec )
{	
	if( glm::dot( rvec, up ) < 0 )
	{
		other.position -= rvec;
		rvec = glm::vec2( 0,0 );
	}
	
	float r = glm::length2( rvec );
	if( r < distanceSquared )
	{
		// offset the particle from the plane
		other.position += up*(distance-sqrtf(r));
		
		// if the particle is moving in the direction oposite of the plane do nothing
		if( glm::dot( other.velocity, up ) > 0 ) return;

		// else, change the direction of the velocity vector (mirror it around the plane axis)
		glm::vec2 axis = direction;
		glm::vec2 v = other.velocity;//.normalized();
		float cosine = glm::dot( axis, v ); 
		if( cosine < 0 )
		{
			axis = -direction;
			cosine = -cosine;
		}
		glm::vec2 newVelocity = axis *( 2*cosine ) - v;
		other.velocity = newVelocity*0.8f;// * other.velocity.norm();	
		other.force -= newVelocity*other.mass;
	}
}

glm::vec2 SPHPlaneInteractor2d::directionTo( SPHParticle2d& other )
{
	glm::vec2 toStart = other.position - start;		
	float t = glm::dot( toStart, direction );
	glm::vec2 projected = start + direction * t;

	return other.position - projected;
}

// TODO: remove this drawing style
void SPHPlaneInteractor2d::draw()
{
	glm::vec2 ns, ne;
	ns = start + direction  *0.5f;
	ne = ns + up;
	
	glBegin( GL_LINES );
		glColor3f( 1,0,0 );
		glVertex2f( start.x, start.y );
		glVertex2f( ns.x + direction.x*0.5f, ns.y+direction.y*0.5f );

		glColor3f( 0,1,0 );
		glVertex2f( ns.x, ns.y );
		glVertex2f( ne.x, ne.y );

	glEnd();
}