
#include "SPHLineInteractor2d.h"
#include "SPHParticle2d.h"
#include <gl\glew.h>
#include <glm\geometric.hpp>
#include <glm\gtx\norm.hpp>

SPHLineInteractor2d::SPHLineInteractor2d( glm::vec2 start, glm::vec2 end )
{
	this->start = start;
	this->end = end;
	direction = end - start;
	length = glm::length( direction );
	lengthSquared = length * length;
}

void SPHLineInteractor2d::applyDensity( SPHParticle2d& other, glm::vec2 rvec )
{
	
}

void SPHLineInteractor2d::applyForce( SPHParticle2d& other, glm::vec2 rvec )
{

}

void SPHLineInteractor2d::enforceInteractor( SPHParticle2d& other, glm::vec2 rvec )
{
	if( glm::length2( rvec ) < 0.5 ){
		other.position -= rvec;
		other.velocity = glm::vec2(0,0);
		
		if( glm::dot(rvec,direction) == 0 )
		{
			glm::vec2 v = other.velocity;
			glm::vec2 n = glm::normalize( direction );
			//v.normalize();
			glm::vec2 newV = n * glm::dot( n, v ) * 2.0f - v;
			//newV = newV * other.velocity.norm();
			other.velocity = newV;
		}else{
			glm::vec2 v = other.velocity;
			glm::vec2 n = glm::normalize( rvec );
			//v.normalize();
			glm::vec2 newV = n * glm::dot( n, v ) * 2.0f - v;
			//newV = newV * other.velocity.norm();
			other.velocity = newV;
		}
	/*	
		glm::vec2 v = -other.velocity;
		glm::vec2 n = rvec;
		n.normalize();
		//v.normalize();
		glm::vec2 newV = n * n.dotProduct(v) * 2 - v;
		//newV = newV * other.velocity.norm();
		other.velocity = newV;
		/**/
		/*
		float x = other.velocity.x * other.velocity.x * other.mass / 2;
		float y = other.velocity.y * other.velocity.y * other.mass / 2;
		
		other.force += glm::vec2( x, y );/**/
	}		
}

glm::vec2 SPHLineInteractor2d::directionTo( SPHParticle2d& other )
{
//	last0 = other.position;

	glm::vec2 toStart = other.position - start;
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

	glm::vec2 projected = start + direction * t;
	//last1 = other.position - projected;
	return other.position - projected;
}

void SPHLineInteractor2d::draw()
{
	// TODO: draw interactors
}