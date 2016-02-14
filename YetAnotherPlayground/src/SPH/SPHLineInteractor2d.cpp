
#include "SPHLineInteractor2d.h"
#include "SPHParticle2d.h"
#include <gl\glew.h>
#include <glm\geometric.hpp>
#include <glm\gtx\norm.hpp>

SPHLineInteractor2d::SPHLineInteractor2d( vec2f start, vec2f end )
{
	this->start = start;
	this->end = end;
	direction = end - start;
	length = glm::length( direction );
	lengthSquared = length * length;
}

void SPHLineInteractor2d::applyDensity( SPHParticle2d& other, vec2f rvec )
{
	
}

void SPHLineInteractor2d::applyForce( SPHParticle2d& other, vec2f rvec )
{

}

void SPHLineInteractor2d::enforceInteractor( SPHParticle2d& other, vec2f rvec )
{
	if( glm::length2( rvec ) < 0.5 ){
		other.position -= rvec;
		other.velocity = vec2f(0,0);
		
		if( glm::dot(rvec,direction) == 0 )
		{
			vec2f v = other.velocity;
			vec2f n = glm::normalize( direction );
			//v.normalize();
			vec2f newV = n * glm::dot( n, v ) * 2.0f - v;
			//newV = newV * other.velocity.norm();
			other.velocity = newV;
		}else{
			vec2f v = other.velocity;
			vec2f n = glm::normalize( rvec );
			//v.normalize();
			vec2f newV = n * glm::dot( n, v ) * 2.0f - v;
			//newV = newV * other.velocity.norm();
			other.velocity = newV;
		}
	/*	
		vec2f v = -other.velocity;
		vec2f n = rvec;
		n.normalize();
		//v.normalize();
		vec2f newV = n * n.dotProduct(v) * 2 - v;
		//newV = newV * other.velocity.norm();
		other.velocity = newV;
		/**/
		/*
		float x = other.velocity.x * other.velocity.x * other.mass / 2;
		float y = other.velocity.y * other.velocity.y * other.mass / 2;
		
		other.force += vec2f( x, y );/**/
	}		
}

vec2f SPHLineInteractor2d::directionTo( SPHParticle2d& other )
{
//	last0 = other.position;

	vec2f toStart = other.position - start;
	//last1 = toStart;
	if( length == 0 ){		
		return toStart;
	}
	
	float t = glm::dot( toStart, direction ) / lengthSquared;
	if( t < 0 ) return toStart;
	if( t > 1 ){
		//last1 = other.position - end;
		return other.position - end;
	}

	vec2f projected = start + direction * t;
	//last1 = other.position - projected;
	return other.position - projected;
}

void SPHLineInteractor2d::draw()
{
	vec2f ns, ne;
	ns = start + direction * 0.5f;
	ne = ns + vec2f( direction.y, -direction.x ) * 0.1f;
	
	glBegin( GL_LINES );
		glColor3f( 1,0,0 );
		glVertex2f( start.x, start.y );
		glVertex2f( end.x, end.y );

		glColor3f( 0,1,0 );
		glVertex2f( ns.x, ns.y );
		glVertex2f( ne.x, ne.y );

	glEnd();
}