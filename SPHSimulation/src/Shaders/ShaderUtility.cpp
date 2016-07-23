
#include "ShaderUtility.h"
#include "Utility.h"
#include <string>
#include <iostream>

using namespace std;

GLenum getShaderTypeFromFilePath( const std::string& filePath )
{
	string extension = getFileExtension( filePath );	
	return getShaderTypeFromName( extension );
}

GLenum getShaderTypeFromName( string name )
{
	if( equals( name, "frag" ) )
	{
		return GL_FRAGMENT_SHADER;
	}else if( equals( name, "geom" ) )
	{
		return GL_GEOMETRY_SHADER;
	}else if( equals( name, "vert" ) )
	{
		return GL_VERTEX_SHADER;
	}else{
		// Unknown shader type, default to vertex shader
		return GL_VERTEX_SHADER;
	}
}

string getShaderNameFromType( GLenum type )
{
	string name;
	switch( type )
	{
		case GL_FRAGMENT_SHADER: name = "frag"; break;
		case GL_GEOMETRY_SHADER: name = "geom"; break;
		case GL_VERTEX_SHADER:	 name = "vert"; break;
		default:				 name = "unknown"; break;
	}
	return name;
}

void printShaderObjectLog(GLuint object)
{
	GLboolean isShader = glIsShader(object);
	int logLength = 0;
	if( isShader == GL_TRUE )
	{
		glGetShaderiv(object,GL_INFO_LOG_LENGTH,&logLength);		
	}
	else
	{
		glGetProgramiv(object,GL_INFO_LOG_LENGTH,&logLength);		
	}

	int writtenCount = 0;
	char *log = new char[logLength];
 
	if( isShader )
	{
		glGetShaderInfoLog(object, logLength, &writtenCount, log);
	}
	else
	{
		glGetProgramInfoLog(object, logLength, &writtenCount, log);
	}
 
	if (logLength > 0)
	{
		cout << log << endl;
		delete [] log;
	}
}