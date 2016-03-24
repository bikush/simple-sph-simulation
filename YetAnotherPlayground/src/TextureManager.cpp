
#include "TextureManager.h"
#include "IL/il.h"
#include "IL/ilut.h"

using namespace std;

TextureManager TextureManager::instance;

TextureManager::TextureManager() : isInitialized(false)
{
}

TextureManager::~TextureManager()
{
	deleteAllTextures();
}

GLuint TextureManager::loadTexture( const char* fileName )
{
	if (!isInitialized) {
		ilInit();
		ilutInit();
		ilutRenderer(ILUT_OPENGL);
		isInitialized = true;
	}

	string sFileName = string(fileName);
	map<string, GLuint>::iterator found = loaded.find( sFileName );

	if( found != loaded.end() )
		return found->second;

	glEnable(GL_TEXTURE_2D);

	size_t size = sFileName.length() + 1;
	wchar_t* wc_imgPath = new wchar_t[size];
	size_t outSize;
	mbstowcs_s(&outSize, wc_imgPath, size, sFileName.c_str(), size - 1);

	// TODO: unhack this wc crap
	GLuint tex =  ilutGLLoadImage(wc_imgPath);
	
	loaded[ sFileName ] = tex;

	glDisable(GL_TEXTURE_2D);
	return tex;
}

void TextureManager::deleteAllTextures()
{	
	GLuint *textureNames = new GLuint[ loaded.size() ];	
	unsigned int index = 0;

	map<string, GLuint>::iterator pair = loaded.begin();
	while( pair != loaded.end() && index < loaded.size() )
	{
		textureNames[index] = (*pair).second;	
		index++;
		pair++;
	}
	glDeleteTextures( index, textureNames );
	delete [] textureNames;
	loaded.clear();
}