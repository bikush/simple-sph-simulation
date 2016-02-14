
#ifndef _VECTOR2D_C
#define _VECTOR2D_C

#include "Vector2D.h"

#ifndef GLM_DATA

#define NORMALIZATION_BOUND 1E-5

#include "Utility.h"

template<class T>
_Vector2D<T>::_Vector2D(){
	x=(T)0; y=(T)0;
}

template<class T>
_Vector2D<T>::_Vector2D(T _x, T _y){
	x = _x;
	y = _y;		
}

template<class T>
_Vector2D<T>& _Vector2D<T>::operator=(_Vector2D<T> in)
{
	x = in.x;
	y = in.y;
	return *this;
}

template<class T>
void _Vector2D<T>::operator/=(T in){
	this->x /= in;
	this->y /= in;
}

template<class T>
void _Vector2D<T>::operator+=(_Vector2D<T> in){
	this->x += in.x;
	this->y += in.y;
}

template<class T>
void _Vector2D<T>::operator-=(_Vector2D<T> in){
	this->x -= in.x;
	this->y -= in.y;
}

template<class T>
void _Vector2D<T>::operator*=(_Vector2D<T> in){
	this->x *= in.x;
	this->y *= in.y;
}

template<class T>
_Vector2D<T> _Vector2D<T>::operator+(_Vector2D<T> in){
	return _Vector2D<T>(	this->x + in.x,
				this->y + in.y );
}

template<class T>
_Vector2D<T> _Vector2D<T>::operator-(_Vector2D<T> in){
	return _Vector2D<T>(	this->x - in.x,
				this->y - in.y );
}

template<class T>
_Vector2D<T> _Vector2D<T>::operator*(T in){
	return _Vector2D<T>(	this->x * in,
				this->y * in );
}

template<class T>
_Vector2D<T> _Vector2D<T>::operator/(T in){
	return _Vector2D<T>(	this->x / in,
				this->y / in );
}

template<class T>
_Vector2D<T> _Vector2D<T>::operator-(){
	return _Vector2D<T>( - this->x, - this->y );
}

template<class T>
T _Vector2D<T>::norm(){
	return (T) sqrtf( this->dotProduct(*this) );
}

template<class T>
T _Vector2D<T>::normSquared(){
	return (T) this->dotProduct(*this);
}

template<class T>
void _Vector2D<T>::normalize(){
	T norm = this->norm();
	if(norm > NORMALIZATION_BOUND || norm < -NORMALIZATION_BOUND)
		this->operator/=(norm);	
}

template<class T>
_Vector2D<T> _Vector2D<T>::normalized(){
	_Vector2D<T> out = _Vector2D<T>( *this );
	T norm = out.norm();
	if(norm > NORMALIZATION_BOUND || norm < -NORMALIZATION_BOUND)
		out/=(norm);
	return out;
}

template<class T>
T _Vector2D<T>::dotProduct( _Vector2D<T> drugi ){
	return x*drugi.x + y*drugi.y;
}
	
template<class T>
_Vector2D<T> _Vector2D<T>::projectOn( _Vector2D<T> in ){
	return operator*( (dotProduct(in)/( normSquared() )) );
}

template<class T>
T _Vector2D<T>::maxPart()
{
	if( x > y )
	{
		return x;
	}else{
		return y;
	}

}

template <class T>
istream& operator>>( istream &is, _Vector2D<T> &t ){
	is >> t.x >> t.y;
	return is;
}

template<class T>
void _Vector2D<T>::containWithin( _Vector2D<T> lower, _Vector2D<T> upper )
{
	x = contain( x, lower.x, upper.x );
	y = contain( y, lower.y, upper.y );
}

template<class T>
bool _Vector2D<T>::operator==( _Vector2D<T> in )
{
	return x == in.x && y == in.y;
}

template<class T>
void _Vector2D<T>::set( T x, T y )
{
	this->x = x;
	this->y = y;
}

#endif

#endif