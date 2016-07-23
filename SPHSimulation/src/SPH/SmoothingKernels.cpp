
#include "SmoothingKernels.h"
#include <math.h>
#include <glm\geometric.hpp>

#define PI 3.1415926f

iKernel* KernelBuilder::getKernel( std::string name, float smoothingLength )
{
	KernelType type = KernelType::DEFAULT;
	if( name.compare( "KernelPoly6" ) == 0 )
	{
		type = KernelType::POLY6;
	}
	else if( name.compare( "KernelSpiky" ) == 0 )
	{
		type = KernelType::SPIKY;
	}
	else if( name.compare( "KernelViscosity" ) == 0 )
	{
		type = KernelType::VISCOSITY;
	}

	return getKernel( type, smoothingLength );
}

iKernel* KernelBuilder::getKernel( KernelType type, float smoothingLength )
{
	iKernel* out=0;
	switch( type )
	{
		case POLY6: 
			out = new KernelPoly6( smoothingLength );
			break;
		case SPIKY: 
			out = new KernelSpiky( smoothingLength );
			break;
		case VISCOSITY: 
			out = new KernelViscosity( smoothingLength );
			break;
		default:
			out = new KernelPoly6( smoothingLength );
	}
	return out;
}

KernelPoly6::KernelPoly6( float h )
{
	adjustSmoothingLength( h );
}

inline void KernelPoly6::adjustSmoothingLength( float h )
{
	this->h = h;
	baseFactor = 315.0f / (64 * PI * pow( h, 9 ));
	// CHECK
	// this gives a negative gradient graph, in Muller the graph is positive...
	gradientFactor = 6.0f*baseFactor;// -6*315 / (64 * PI * pow( h, 9 ) );
	laplacianFactor = 24.0f*baseFactor;//24*315 / (64 * PI * pow( h, 9 ) );
	hSquared = h*h;
}

inline float KernelPoly6::base( float r )
{
	r = abs(r);
	if( r < h )
	{
		return baseFactor * ( pow(hSquared - r*r, 3) );
	}else
	{
		return 0.0;
	}
}

inline glm::vec2 KernelPoly6::gradient( glm::vec2 rvec )
{
	
	float rSq = glm::dot(rvec, rvec);
	if( rSq >= 0 && rSq < hSquared )
	{
		return rvec*( gradientFactor*pow(hSquared - rSq, 2) );
	}else
	{
		return glm::vec2(0,0);
	}
}

inline glm::vec3 KernelPoly6::gradient( glm::vec3 rvec )
{
	float rSq = glm::dot(rvec, rvec);
	if( rSq >= 0 && rSq < hSquared )
	{
		return rvec*( gradientFactor*pow(hSquared - rSq, 2) );
	}else
	{
		return glm::vec3(0,0,0);
	}
}

inline float KernelPoly6::laplacian( float r )
{
	float rSq = r*r;
	if( rSq >= 0 && rSq < hSquared )
	{
		return laplacianFactor * (hSquared - rSq) * ( -0.75f*(hSquared - rSq) + rSq )/(3*PI); // 3PI is experimental constant
	}else
	{
		return 0.0;
	}
}

/****************
 * Kernel Spiky *
 ****************/

KernelSpiky::KernelSpiky( float h )
{
	adjustSmoothingLength( h );
}

inline void KernelSpiky::adjustSmoothingLength( float h )
{
	this->h = h;
	baseFactor = 15.0f / ( PI * pow( h, 6 ) );
	gradientFactor = 3 * baseFactor;// -45.0 / ( PI * pow( h, 6 ) );
	laplacianFactor = -6 * baseFactor;// 90 / ( PI * pow( h, 6 ) );
}

inline float KernelSpiky::base( float r )
{
	r = abs(r);
	if( r < h )
	{
		return baseFactor * pow(h-r, 3);
	}else
	{
		return 0.0;
	}
}

inline glm::vec2 KernelSpiky::gradient( glm::vec2 rvec )
{
	float rSq = glm::dot(rvec, rvec);
	if( rSq < h*h )
	{
		rSq = sqrtf(rSq);
		return rvec *( gradientFactor * pow(h-rSq, 2) / rSq );
	}else
	{
		return glm::vec2();
	}
}

inline glm::vec3 KernelSpiky::gradient( glm::vec3 rvec )
{
	float rSq = glm::dot(rvec, rvec);
	if( rSq < h*h )
	{
		rSq = sqrtf(rSq);
		return rvec *( gradientFactor * pow(h-rSq, 2) / rSq );
	}else
	{
		return glm::vec3();
	}
}

inline float KernelSpiky::laplacian( float r )
{
	r = abs(r);
	if( r < h )
	{
		if( r<0.0001f ) r = 0.0001f;
		return laplacianFactor *(h*h/r-3*h+2*r);
	}else
	{
		return 0.0f;
	}
}

/********************
 * Kernel Viscosity *
 ********************/
KernelViscosity::KernelViscosity( float h )
{
	adjustSmoothingLength( h );
}

inline void KernelViscosity::adjustSmoothingLength( float h )
{
	this->h = h;
	baseFactor = 15 / ( 2 * PI * pow( h, 3 ) );
	gradientFactor = -15 / ( 2 * PI * pow( h, 3 ) );
	laplacianFactor = 45 / ( PI * pow( h, 6 ) );
}

inline float KernelViscosity::base( float r )
{
	r = abs(r);
	if( r < h )
	{
		float rh = r/h;
		return baseFactor * ( -(rh*rh*rh/2) + rh*rh + 1/(rh*2) - 1  );
	}else
	{
		return 0.0;
	}
}

inline glm::vec2 KernelViscosity::gradient( glm::vec2 rvec )
{
	float r = glm::length(rvec);
	if( r > -h && r < h )
	{
		return  rvec*(( gradientFactor * ( -(3*r*r/(2*h*h*h)) + 2*r/(h*h) - h/(2*r*r)  ) )/r);
	}else
	{
		return glm::vec2();
	}
}

inline glm::vec3 KernelViscosity::gradient( glm::vec3 rvec )
{
	float r = glm::length(rvec);
	if( r > -h && r < h )
	{
		return  rvec*(( gradientFactor * ( -(3*r*r/(2*h*h*h)) + 2*r/(h*h) - h/(2*r*r)  ) )/r);
	}else
	{
		return glm::vec3();
	}
}

inline float KernelViscosity::laplacian( float r )
{
	r = abs(r);
	if( r > -h && r < h )
	{
		return laplacianFactor * ( h-r )/h;
	}else
	{
		return 0.0;
	}
}
/*
float KernelSplineGaussian::base( float h, float r )
{
	if( r > 0 && r < h )
	{
		return (1 - 3*pow( r/h, 2 )/2 + 3*pow(r/h,3)/4)/(PI * h);
	}else if( r >= h && r <= 2*h )
	{
		return (pow( 2-r/h, 3)/4)/(PI * h);
	}else
	{
		return 0.0;
	}
}

glm::vec2 KernelSplineGaussian::gradient( float h, glm::vec2 rvec )
{
	float r = rvec.norm();
	if( r > 0 && r < h )
	{
		return rvec*(( ( 1 - 3*(r/(h*h)) + 9*pow(r/h,2)/(4*h) )/(PI * h))/r);
	}else if( r >= h && r <= 2*h )
	{
		return rvec*(((-3*pow( 2-r/h, 2)/(4*h))/(PI * h))/r);
	}else
	{
		return glm::vec2();
	}
}

glm::vec3 KernelSplineGaussian::gradient( float h, glm::vec3 rvec )
{
	float r = rvec.norm();
	if( r > 0 && r < h )
	{
		return rvec*(( ( 1 - 3*(r/(h*h)) + 9*pow(r/h,2)/(4*h) )/(PI * h))/r);
	}else if( r >= h && r <= 2*h )
	{
		return rvec*(((-3*pow( 2-r/h, 2)/(4*h))/(PI * h))/r);
	}else
	{
		return glm::vec3();
	}
}

float KernelSplineGaussian::laplacian( float h, float r )
{
	if( r > 0 && r < h )
	{
		return ( 1 - 3/(h*h) + 9*(r/(2*h*h*h)) )/(PI * h);
	}else if( r >= h && r <= 2*h )
	{
		return (-3*( 2-r )/(2*h*h*h))/(PI * h);
	}else
	{
		return 0.0;
	}
}

float KernelDesburn::base( float h, float r )
{
	if( r > 0 && r < 2*h )
	{
		return 15 * ( pow( 2-r/h, 3 )  ) / ( PI * pow( 4*h, 3 ) );
	}else
	{
		return 0.0;
	}
}

glm::vec2 KernelDesburn::gradient( float h, glm::vec2 rvec )
{
	float r = rvec.norm();
	if( r > 0 && r < 2*h )
	{
		return rvec*(( 15 * ( -3*pow( 2-r/h, 2 )/h  ) / ( PI * pow( 4*h, 3 ) ))/r);
	}else
	{
		return glm::vec2();
	}
}

glm::vec3 KernelDesburn::gradient( float h, glm::vec3 rvec )
{
	float r = rvec.norm();
	if( r > 0 && r < 2*h )
	{
		return rvec*(( 15 * ( -3*pow( 2-r/h, 2 )/h  ) / ( PI * pow( 4*h, 3 ) ))/r);
	}else
	{
		return glm::vec3();
	}
}

float KernelDesburn::laplacian( float h, float r )
{
	if( r > 0 && r < 2*h )
	{
		return 15 * ( -6*( 2-r )/(h*h*h)  ) / ( PI * pow( 4*h, 3 ) );
	}else
	{
		return 0.0;
	}
}*/