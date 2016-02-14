#pragma once
#ifndef SHADER_BASE_H
#define SHADER_BASE_H

#include <GL\glew.h>
#include <iostream>

class ShaderBase
{
	GLuint shaderID;
	void printLog();

public:
	ShaderBase( const std::string& filePath );
	~ShaderBase();

	GLuint getShaderID();	
};

#endif