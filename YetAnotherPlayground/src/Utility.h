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

template<class T> T readString( std::string &s )
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

template<typename... Args>
std::size_t sizeof_all() {
	size_t sizes[] = { sizeof(Args)... };
	int count = sizeof...(Args);
	size_t total = 0;
	for (int i = 0; i < count; i++) {
		total += sizes[i];
	}
	return total;
}

void print(const glm::mat4& m);

std::vector<std::string> split( const char* line, char splitter );
std::vector<std::string> split( std::string line, char splitter );
std::string join( std::vector<std::string> parts, std::string joiner );
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