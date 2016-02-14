#pragma once
#ifndef SHADER_PROGRAM_H
#define SHADER_PROGRAM_H

#include <vector>
#include <map>
#include <string>
#include <GL\glew.h>
#include <iostream>
#include <map>

class ShaderBase;

class ShaderProgram
{
private:
	static std::map< std::string, ShaderProgram* > shaderCache;
	
	bool removeFromCache( ShaderProgram* program );
	void removeFromCache( ShaderProgram** program );

public:
	static ShaderProgram* CreateBaseShader( );
	static ShaderProgram* CreateBase3DShader();
	static ShaderProgram* CreateShader( const std::string& vertex, const std::string& fragment, bool doLink = true );
	static ShaderProgram* CreateShader( const std::string& vertex, const std::string& geometry, const std::string& fragment, bool doLink = true );
	
private:
	ShaderProgram( );	
	ShaderProgram( const std::string& vertex, const std::string& fragment, bool doLink = true );
	ShaderProgram( const std::string& vertex, const std::string& geometry, const std::string& fragment, bool doLink = true );
	~ShaderProgram();

	void initMembers( const std::string& vertex, const std::string& geometry, const std::string& fragment, bool doLink );
	void printLog();	
	
	bool addShader( const std::string& shaderPath );
	void detachAllShaders();
	void attach();
	void link();

	GLuint programID;
	std::vector< ShaderBase* > attachedShaders;
	bool linkedOk;

public:
	GLuint getProgramID();
	void turnOn();
	static void turnOff();


// UNIFORMS
private:
	std::map< std::string, GLint > uniformLocations;	
	void populateActiveUniforms( bool output );

public:
	GLint getUniform( const std::string& uniformName );

	bool setUniformV3( const std::string& name, float x, float y, float z );
	bool setUniformV4( const std::string& name, float x, float y, float z, float w );
	bool setUniformM3( const std::string& name, float* m );
	bool setUniformM4( const std::string& name, float* m );
	bool setUniformF(  const std::string& name, float f  );
	bool setUniformI(  const std::string& name, int i    );
	bool setUniformB(  const std::string& name, bool b   );

// ATTRIBUTES
private:
	std::map< std::string, GLint > attributeLocations;
	void populateActiveAttributes( bool output );

public:
	GLint getAttribute( const std::string& attributeName );
};


#endif