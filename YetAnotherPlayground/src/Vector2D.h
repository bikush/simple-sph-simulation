#pragma once
#ifndef _VECTOR_2D_H
#define _VECTOR_2D_H

#define GLM_DATA

#ifdef GLM_DATA

	#include <glm\vec2.hpp>
	typedef glm::vec2 vec2f;

#else

	#include <istream>
	#include <math.h>
	//#include <gl\glew.h>

	using namespace std;

	template <class T>
	struct _Vector2D{
		T x;
		T y;

		_Vector2D();
		_Vector2D(T _x, T _y);

		_Vector2D& operator=(_Vector2D in);
		void operator/=(T in);
		void operator+=(_Vector2D in);
		void operator-=(_Vector2D in);
		void operator*=(_Vector2D in);

		_Vector2D operator+(_Vector2D in);
		_Vector2D operator-(_Vector2D in);
		_Vector2D operator*(T in);
		_Vector2D operator/(T in);

		_Vector2D operator-();

		bool operator==(_Vector2D in);

		T norm();
		T normSquared();
		void normalize();
		_Vector2D normalized();
		T dotProduct( _Vector2D drugi );
		void set( T x, T y );
	
		_Vector2D projectOn( _Vector2D in );
		T maxPart();

		template <class T1>
		friend istream& operator>>( istream &is, _Vector2D<T1> &t );

		void containWithin( _Vector2D lower, _Vector2D upper );
	
	};

	#include "Vector2D.cpp"

	typedef _Vector2D<float> vec2f;

#endif

#endif
