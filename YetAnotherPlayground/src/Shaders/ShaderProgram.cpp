
#include "ShaderProgram.h"
#include "ShaderBase.h"
#include "ShaderUtility.h"


using namespace std;

/***********
 *  CACHE  *
 ***********/

map< string, ShaderProgram* > ShaderProgram::shaderCache;

bool ShaderProgram::removeFromCache( ShaderProgram* program )
{
	for( auto shaderIt = shaderCache.begin(); shaderIt != shaderCache.end(); ++shaderIt )
	{
		if( shaderIt->second == program )
		{
			delete program;
			shaderCache.erase( shaderIt );
			return true;;
		}
	}
	return false;
}

void ShaderProgram::removeFromCache( ShaderProgram** program )
{
	if( removeFromCache( *program ) )
	{
		*program = nullptr;
	}
}

ShaderProgram* ShaderProgram::CreateBaseShader( )
{
	return CreateShader( "data/shaders/baseShader.vert", "", "data/shaders/baseShader.frag", true );
}

ShaderProgram* ShaderProgram::CreateBase3DShader()
{
	return CreateShader("data/shaders/baseShader3D.vert", "", "data/shaders/baseShader3D.frag", true);
}

ShaderProgram* ShaderProgram::CreateShader( const string& vertex, const string& fragment, bool doLink )
{
	return CreateShader( vertex, "", fragment, doLink );
}

ShaderProgram* ShaderProgram::CreateShader( const string& vertex, const string& geometry, const string& fragment, bool doLink )
{
	std::string entry = vertex + geometry + fragment;
	auto shaderIt = shaderCache.find( entry );
	if( shaderIt == shaderCache.end() )
	{
		ShaderProgram* program = new ShaderProgram( vertex, geometry, fragment, doLink );
		shaderCache[entry] = program;
		return program;
	}
	return shaderIt->second;
}


/***********
 * PRIVATE *
 ***********/

ShaderProgram::ShaderProgram()
{
	initMembers( "", "", "", false );
}

ShaderProgram::ShaderProgram( const string& vertex, const string& fragment, bool doLink ) 
{
	initMembers( vertex, "", fragment, doLink );
}

ShaderProgram::ShaderProgram( const string& vertex, const string& geometry, const string& fragment, bool doLink )
{
	initMembers( vertex, geometry, fragment, doLink );
}

ShaderProgram::~ShaderProgram()
{
	detachAllShaders();
	glDeleteProgram( programID );	
}

void ShaderProgram::initMembers( const string& vertex, const string& geometry, const string& fragment, bool doLink )
{
	//attachedShaders = vector<ShaderBase*>();
	programID = glCreateProgram();
	if( programID == 0 )
	{
		cerr << "Shader Program not created!" << endl;
		// TODO: throw exception?
		return;
	}
	linkedOk = false;
	//uniformLocations = map< string, GLint >();

	cout << " Compiling shader sources: " << endl;		
	cout << "* VERTEX: " << vertex << endl;
	addShader( vertex );
	cout << "* GEOMETRY: " << geometry << endl;
	addShader( geometry );
	cout << "* FRAGMENT: " << fragment << endl;
	addShader( fragment );
			
	if( doLink )
	{
		attach();
		link();
	}
}

void ShaderProgram::printLog( )
{
	int logLength = 0;
	glGetProgramiv(programID,GL_INFO_LOG_LENGTH,&logLength);		
		
	if (logLength > 0)
	{
		int writtenCount = 0;
		char *log = new char[logLength];
 		glGetProgramInfoLog(programID, logLength, &writtenCount, log);
		
		cout << "ShaderProgram: " << programID << " -> log: " ;
		if (writtenCount > 0)
		{
			cout << log << endl;			
		}
		else
		{
			cout << "nothing written in the log!" << endl;
		}

		delete [] log;
	}
	else
	{
		cout << "ShaderProgram: " << programID << " -> nothing to log!" << endl;
	}
}

bool ShaderProgram::addShader( const string& shaderPath )
{
	if( !linkedOk && shaderPath.length() > 0 )
	{
		// TODO: handle shader base creation failure, currently it just does exit(-1)
		attachedShaders.push_back( new ShaderBase( shaderPath ) );
		return true;
	}
	return false;
}

void ShaderProgram::detachAllShaders()
{
	for(auto shader = attachedShaders.begin(); shader!=attachedShaders.end(); shader++)
	{
		glDetachShader( programID, (*shader)->getShaderID() );
		delete *shader;		
	}
	attachedShaders.clear();
	linkedOk = false;
}

void ShaderProgram::attach()
{
	cout << " Attaching shaders:"<<endl;
	for(unsigned int i=0; i<attachedShaders.size(); i++)
	{
		glAttachShader(programID, attachedShaders[i]->getShaderID() );
	}	
}

void ShaderProgram::link()
{
	cout << " Linking shaders:"<<endl;	
	glLinkProgram(programID);
	
	GLint status;
	glGetProgramiv( programID, GL_LINK_STATUS, &status );
	if( status == GL_FALSE )
	{
		printLog( );
		linkedOk = false;
	}
	else
	{
		printLog( );
		linkedOk = true;

		populateActiveAttributes( true );
		populateActiveUniforms( true );
	}
}

GLuint ShaderProgram::getProgramID()
{
	return programID;
}

void ShaderProgram::turnOn()
{
	if(linkedOk)
	{
		glUseProgram( programID );
	}
}

void ShaderProgram::turnOff()
{
	glUseProgram( 0 );
}


/*
 * UNIFORMS
 */

void ShaderProgram::populateActiveUniforms( bool output )
{
	// Populate uniform variables
	GLint nUniforms;
	GLint maxLen;
	glGetProgramiv( programID, GL_ACTIVE_UNIFORM_MAX_LENGTH, &maxLen);
	glGetProgramiv( programID, GL_ACTIVE_UNIFORMS, &nUniforms);

	GLchar* name = new char[ maxLen ];
	GLint size;
	GLsizei written;
	GLenum type;
	if( output )
	{
		cout << "Shader Program " << programID << " uniforms:"<< endl << "Name -> location" << endl;
	}
	for( int i = 0; i < nUniforms; ++i ) 
	{
		glGetActiveUniform( programID, i, maxLen, &written, &size, &type, name );
		uniformLocations[ name ] = glGetUniformLocation(programID, name);
		if( output )
		{
			cout << name << " -> " << uniformLocations[ name ] << endl;
		}
	}
	delete [] name;
}

GLint ShaderProgram::getUniform( const string& uniformName )
{
	map< string, GLint >::iterator found = uniformLocations.find( uniformName );
	if( found == uniformLocations.end() )
	{
		GLint location = glGetUniformLocation(programID, uniformName.c_str());
		if( location < 0 )
			cout << "Unknown uniform: " << uniformName << endl;
		// TODO: logging and catching such errors as these
		uniformLocations[ uniformName ] = location;
		return location;
	}
	else
	{
		return (*found).second;
	}
}

bool ShaderProgram::setUniformV3( const string& name, float x, float y, float z )
{
	map< string, GLint >::iterator found = uniformLocations.find( name );
	if( found != uniformLocations.end() )
	{
		glUniform3f((*found).second, x, y, z);
		return true;
	}else
	{
		GLint location = glGetUniformLocation(programID, name.c_str());
		if( location != -1 )
		{
			glUniform3f(location, x, y, z);
			return true;
		}
	}
	return false;
}

bool ShaderProgram::setUniformV4( const string& name, float x, float y, float z, float w )
{
	map< string, GLint >::iterator found = uniformLocations.find( name );
	if( found != uniformLocations.end() )
	{
		glUniform4f((*found).second, x, y, z, w);
		return true;
	}
	return false;
}

bool ShaderProgram::setUniformM3( const string& name, float* m )
{
	map< string, GLint >::iterator found = uniformLocations.find( name );
	if( found != uniformLocations.end() )
	{
		glUniformMatrix3fv((*found).second, 1, GL_FALSE, m);
		return true;
	}	
	return false;
}

bool ShaderProgram::setUniformM4( const string& name, float* m )
{
	map< string, GLint >::iterator found = uniformLocations.find( name );
	if( found != uniformLocations.end() )
	{
		glUniformMatrix4fv((*found).second, 1, GL_FALSE, m);
		return true;
	}	
	return false;
}

bool ShaderProgram::setUniformF(  const string& name, float f )
{
	map< string, GLint >::iterator found = uniformLocations.find( name );
	if( found != uniformLocations.end() )
	{
		glUniform1f( (*found).second, f);
		return true;
	}
	return false;
}

bool ShaderProgram::setUniformI(  const string& name, int i )
{
	map< string, GLint >::iterator found = uniformLocations.find( name );
	if( found != uniformLocations.end() )
	{
		glUniform1i((*found).second, i);
		return true;
	}
	return false;
}

bool ShaderProgram::setUniformB(  const string& name, bool b )
{
	map< string, GLint >::iterator found = uniformLocations.find( name );
	if( found != uniformLocations.end() )
	{
		glUniform1i((*found).second, b ? 1 : 0 );
		return true;
	}
	return false;
}

/*
 * ATTRIBUTES
 */

void ShaderProgram::populateActiveAttributes( bool output )
{
	GLint maxLength, nAttribs;
	glGetProgramiv(programID, GL_ACTIVE_ATTRIBUTES, &nAttribs);
	glGetProgramiv(programID, GL_ACTIVE_ATTRIBUTE_MAX_LENGTH, &maxLength);

	GLchar* name = new char[ maxLength ];
	GLint written;
	GLint size;
	GLint location; 
	GLenum type;

	if( output )
	{
		cout << "Shader program " << programID << " attributes:" << endl << "@ Index : Name" << endl;
	}
	for( int i = 0; i < nAttribs; i++ ) {
		glGetActiveAttrib( programID, i, maxLength, &written, &size, &type, name );
		location = glGetAttribLocation(programID, name);
		if( output )
		{
			cout << "@ " << location << " : "<< name << endl;
		}
	}
	delete name;
}

GLint ShaderProgram::getAttribute( const string& attributeName )
{
	auto foundIt = attributeLocations.find(attributeName);
	if( foundIt == attributeLocations.end() )
	{
		GLint location = glGetAttribLocation(programID, attributeName.c_str());
		if( location < 0 )
		{
			cout << "Unknown/unspecified attribute " << attributeName << endl;
		}
		attributeLocations[ attributeName ] = location;
		return location;
	}
	else
	{
		return foundIt->second;
	}
}