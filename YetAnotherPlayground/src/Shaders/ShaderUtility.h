#pragma once
#ifndef SHADER_UTILITY_H
#define SHADER_UTILITY_H

#include <GL\glew.h>
#include <string>

GLenum getShaderTypeFromFilePath( const std::string& filePath );
GLenum getShaderTypeFromName( std::string shaderName );
std::string getShaderNameFromType( GLenum type );
void printShaderObjectLog( GLuint object );

#endif