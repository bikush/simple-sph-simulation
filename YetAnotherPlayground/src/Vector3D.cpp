#ifndef _VECTOR_3D_C
#define _VECTOR_3D_C

#include "Vector3D.h"
#include <math.h>

#ifdef GLM_DATA

bool isWithin(vec3f value, vec3f lower, vec3f upper) {
	return
		value.x >= lower.x && value.x <= upper.x &&
		value.y >= lower.y && value.y <= upper.y &&
		value.z >= lower.z && value.z <= upper.z;
}

float minPart(const vec3f& value) {
	return fminf(value.x, fminf(value.y, value.z));
}

float maxPart(const vec3f& value) {
	return fmaxf(value.x, fmaxf(value.y, value.z));
}

vec3f minDim(const vec3f& value) {
	float min = value.x;
	vec3f out(min, 0, 0);
	if (min > value.y)
	{
		min = value.y;
		out.x = 0;
		out.y = value.y;
	}
	if (min > value.z)
	{
		out.x = 0;
		out.y = 0;
		out.z = value.z;
	}
	return out;
}

vec3f minDir(const vec3f& value) {
	float min = value.x;
	vec3f out(1, 0, 0);
	if (min > value.y)
	{
		min = value.y;
		out.x = 0;
		out.y = 1;
	}
	if (min > value.z)
	{
		out.x = 0;
		out.y = 0;
		out.z = 1;
	}
	return out;
}

#else

template<class T>
_Vector3D<T>::_Vector3D(){
	x=(T)0; y=(T)0; z=(T)0;
}

template<class T>
_Vector3D<T>::_Vector3D(T _x, T _y, T _z){
	x = _x;
	y = _y;
	z = _z;
}

template <class T>
_Vector3D<T>& _Vector3D<T>::operator=(_Vector3D<T> in)
{
	x = in.x;
	y = in.y;
	z = in.z;
	return *this;
}

template <class T>
void _Vector3D<T>::operator/=(T in){
	this->x /= in;
	this->y /= in;
	this->z /= in;
}

template <class T>
void _Vector3D<T>::operator/=(_Vector3D<T> in){
	this->x /= in.x;
	this->y /= in.y;
	this->z /= in.z;
}

template <class T>
void _Vector3D<T>::operator*=(_Vector3D<T> in){
	this->x *= in.x;
	this->y *= in.y;
	this->z *= in.z;
}

template <class T>
void _Vector3D<T>::operator*=(T in){
	this->x *= in;
	this->y *= in;
	this->z *= in;
}

template <class T>
void _Vector3D<T>::operator+=(_Vector3D<T> in){
	this->x += in.x;
	this->y += in.y;
	this->z += in.z;
}

template <class T>
void _Vector3D<T>::operator-=(_Vector3D<T> in){
	this->x -= in.x;
	this->y -= in.y;
	this->z -= in.z;
}

template <class T>
_Vector3D<T> _Vector3D<T>::operator+(_Vector3D<T> in){
	return _Vector3D<T>(	this->x + in.x,
				this->y + in.y,
				this->z + in.z );
}

template <class T>
_Vector3D<T> _Vector3D<T>::operator-(_Vector3D<T> in){
	return _Vector3D<T>(	this->x - in.x,
				this->y - in.y,
				this->z - in.z );
}

template <class T>
_Vector3D<T> _Vector3D<T>::operator*(T in){
	return _Vector3D<T>(	this->x * in,
				this->y * in,
				this->z * in );
}

template <class T>
_Vector3D<T> _Vector3D<T>::operator/(T in){
	return _Vector3D<T>(	this->x / in,
				this->y / in,
				this->z / in );
}

template <class T>
_Vector3D<T> _Vector3D<T>::operator/(_Vector3D<T> in){
	return _Vector3D<T>(	this->x / in.x,
				this->y / in.y,
				this->z / in.z );
}

template <class T>
_Vector3D<T> _Vector3D<T>::operator-(){
	return _Vector3D<T>( - this->x, - this->y, - this->z );
}

template <class T>
T _Vector3D<T>::norm(){
	return (T)sqrtf( this->dotProduct(*this) );
}

template <class T>
T _Vector3D<T>::normSquared(){
	return this->dotProduct(*this);
}

template <class T>
void _Vector3D<T>::normalize(){
	T N = this->norm();
	if(N > 0.00001 || N < -0.00001)
		this->operator/=(N);
}

template <class T>
_Vector3D<T> _Vector3D<T>::normalized(){
	_Vector3D<T> out = _Vector3D<T>( *this );
	T N = out.norm();
	if(N > 0.00001 || N < -0.00001)
		out/=(N);
	return out;
}

template <class T>
T _Vector3D<T>::dotProduct( _Vector3D<T> other ){
	return x*other.x + y*other.y + z*other.z;
}

template <class T>
_Vector3D<T> _Vector3D<T>::crossProduct( _Vector3D<T> other ){
	_Vector3D<T> cross;
	cross.x = y*other.z - other.y*z;
	cross.y =-x*other.z + other.x*z;
	cross.z = x*other.y - other.x*y;
	return cross;
}

template<class T>
void _Vector3D<T>::set( T x, T y, T z )
{
	this->x = x;
	this->y = y;
	this->z = z;
}

template<class T>
void _Vector3D<T>::changeSign()
{
	x = -x;
	y = -y;
	z = -z;
}

template<class T>
_Vector3D<T> _Vector3D<T>::use( bool useX, bool useY, bool useZ )
{
	_Vector3D<T> out = _Vector3D<T>();
	if( useX ) out.x = this->x;
	if( useY ) out.y = this->y;
	if( useZ ) out.z = this->z;
	return out;
}

template <class T>
_Vector3D<T> _Vector3D<T>::projectOn( _Vector3D<T> other ){
	float N = norm();
	return operator*( (dotProduct(other)/( N*N )) );
}

template <class T>
_Vector3D<T> _Vector3D<T>::direction(){
	_Vector3D<T> dir = _Vector3D<T>( *this );
	dir.normalize();
	return dir;
}

template <class T>
T _Vector3D<T>::maxPart()
{
	float out = x;
	if( out < y ) out = y;
	if( out < z ) out = z;
	return out;
}

template <class T>
T _Vector3D<T>::minPart()
{
	float out = x;
	if( out > y ) out = y;
	if( out > z ) out = z;
	return out;
}

template <class T>
_Vector3D<T> _Vector3D<T>::minDim(){
	float min = x;
	_Vector3D<T> out(x,0,0);	
	if( min > y )
	{
		min = y;
		out.set(0,y,0);
	}
	if( min > z )
	{
		out.set(0,0,z);
	}
	return out;
}

template <class T>
_Vector3D<T> _Vector3D<T>::minDir(){
	float min = x;
	_Vector3D<T> out(1,0,0);	
	if( min > y )
	{
		min = y;
		out.set(0,1,0);
	}
	if( min > z )
	{
		out.set(0,0,1);
	}
	return out;
}

template <class T>
void _Vector3D<T>::containWithin( _Vector3D<T> lower, _Vector3D<T> upper ){
	x = x < lower.x ? lower.x : ( x > upper.x ? upper.x : x );
	y = y < lower.y ? lower.y : ( y > upper.y ? upper.y : y );
	z = z < lower.z ? lower.z : ( z > upper.z ? upper.z : z );
}

template <class T>
bool _Vector3D<T>::isWithin( _Vector3D<T> lower, _Vector3D<T> upper ){
	if( x < lower.x || x > upper.x ) return false;
	if( y < lower.y || y > upper.y ) return false;
	if( z < lower.z || z > upper.z ) return false;
	return true;
}

template <class T>
istream& operator>>( istream &is, _Vector3D<T> &t ){
	is >> t.x >> t.y >> t.z;
	return is;
}

#endif

#endif