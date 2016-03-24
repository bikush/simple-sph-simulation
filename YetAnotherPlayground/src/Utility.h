#pragma once
#ifndef _UTILITY_H
#define _UTILITY_H

#include <fstream>
#include <vector>
#include <string>
#include <sstream>

#include <glm\mat4x4.hpp>

#define PI 3.1415926f
#define HALFCIRCLE 180

template<class T> void inline containRef( T& input, T lower, T upper ){
	if (input > upper ){
		input = upper;
	}
	if(input<lower){
		input = lower;
	}
}

template<class T> T inline contain( T input, T lower, T upper ){
	if (input > upper ){
		return upper;
	}
	if(input<lower){
		return lower;
	}
	return input;
}

template<class T> T readString( const std::string &s )
{
	std::istringstream stream( s );
	T out;
	stream >> out;
	return out;
}

template<class C>
C inline randFloat(){
	return C(rand())/RAND_MAX;
}

template<class T>
void inline safeDelete(T** pointer)
{
	if( *pointer != nullptr )
	{
		delete *pointer;
		*pointer = nullptr;
	}
}


template <size_t N>
constexpr size_t cexprSum(const int(&values)[N], int index) {
	return index < N ? (values[index] + cexprSum(values, index + 1)) : 0;
}

template<typename... Args>
constexpr std::size_t sizeof_all() {
	return cexprSum({ sizeof(Args)... }, 0);
}

constexpr size_t offsetFromBytes(size_t desired, size_t owned) {
	return desired - owned;
}


void print(const glm::mat4& m);

std::vector<std::string> split( const char* line, char splitter );
std::vector<std::string> split( std::string line, char splitter );
std::string join( const std::vector<std::string>& parts, std::string joiner );
int countChar( const char* line, char c );
bool equals( std::string first, std::string second );

std::string getFilePathPart( const std::string& fileName );
std::string getFileExtension( const std::string& fileName );
void loadFile( std::string& output, const char* filePath );


inline float angleToRadians( float angle )
{
	return angle * PI / HALFCIRCLE;
}

inline float angleFromRadians( float radians )
{
	return radians * HALFCIRCLE / PI;
}

template<class T> T abs( T in )
{
	return in < 0 ? -in : in;
}

#endif