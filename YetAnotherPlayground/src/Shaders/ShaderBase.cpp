
#include "ShaderBase.h"
#include "ShaderUtility.h"
#include "Utility.h"
#include <GL\glew.h>
#include <string>

using namespace std;

ShaderBase::ShaderBase(const std::string& filePath)
{	
	GLenum type = getShaderTypeFromFilePath( filePath );
	shaderID = glCreateShader(type);
	if( shaderID == 0 )
	{
		cerr << "Error creating shader type: " << getShaderNameFromType( type ) << endl;
		exit(-1);
	}

	std::string source = "";
	loadFile( source, filePath.c_str() );
	const GLchar* codeArray[] = {source.c_str()};
	glShaderSource(shaderID, 1, codeArray, NULL);

	glCompileShader(shaderID);
	GLint result;
	glGetShaderiv( shaderID, GL_COMPILE_STATUS, &result );
	if( result == GL_FALSE ) 
	{
		cerr << "Shader compilation failed, shader ID: " << shaderID << endl;
		printLog( );
		exit( -1 );
	}else
	{
		printLog();
	}
}

ShaderBase::~ShaderBase()
{
	glDeleteShader( shaderID );
}

GLuint ShaderBase::getShaderID()
{
	return shaderID;
}

void ShaderBase::printLog()
{
	int logLength;
	glGetShaderiv(shaderID,GL_INFO_LOG_LENGTH,&logLength);		
	
	if( logLength > 0 )
	{
		int writtenCount = 0;	
		char *log = new char[logLength];
 
		glGetShaderInfoLog(shaderID, logLength, &writtenCount, log);
	 
		cout << "Shader: " << shaderID << " -> log: " ;
		if (writtenCount > 0)
		{
			cout << log << endl;			
		}else
		{
			cout << "nothing written in the log!" << endl;
		}

		delete [] log;
	}else
	{
		cout << "Shader: " << shaderID << " -> nothing to log!" << endl;
	}
}