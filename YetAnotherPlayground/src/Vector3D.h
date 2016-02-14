#pragma once
#ifndef _VECTOR_3D_H
#define _VECTOR_3D_H

#define GLM_DATA

#ifdef GLM_DATA

	#include <glm\vec3.hpp>
	typedef glm::vec3 vec3f;

	bool isWithin(vec3f value, vec3f lower, vec3f upper);

	float minPart(const vec3f& value);

	float maxPart(const vec3f& value);

	vec3f minDim(const vec3f& value);

	vec3f minDir(const vec3f& value);

#else

	#include <istream>
	#include <math.h>
	//#include <gl\glew.h>


	using namespace std;

	template <class T>
	struct _Vector3D{

		T x;
		T y;
		T z;

		_Vector3D();
		_Vector3D(T _x, T _y, T _z);

		_Vector3D& operator=(_Vector3D in);
		void operator/=(T in);
		void operator/=(_Vector3D in);
		void operator*=(_Vector3D in);
		void operator*=(T in);
		void operator+=(_Vector3D in);
		void operator-=(_Vector3D in);

		_Vector3D operator+(_Vector3D in);
		_Vector3D operator-(_Vector3D in);
		_Vector3D operator*(T in);	
		_Vector3D operator/(T in);
		_Vector3D operator/(_Vector3D in);

		_Vector3D operator-();

		T norm();
		T normSquared();
		void normalize();
		_Vector3D normalized();
		T dotProduct( _Vector3D drugi );
		void set( T x, T y, T z );
		_Vector3D use( bool useX, bool useY, bool useZ );	
		void changeSign();
	
		_Vector3D crossProduct( _Vector3D E );
		_Vector3D projectOn( _Vector3D in );
		_Vector3D direction();

		T maxPart();
		T minPart();

		_Vector3D minDim();
		_Vector3D minDir();
	
		void containWithin( _Vector3D lower, _Vector3D upper );
		bool isWithin( _Vector3D lower, _Vector3D upper );

		template<class T1>
		friend istream& operator>>( istream &is, _Vector3D<T1> &t );
	};

	typedef _Vector3D<float> vec3f;

	#include "Vector3D.cpp"

#endif

#endif
