
#include "TextureManager.h"
#include "IL/il.h"
#include "IL/ilut.h"

using namespace std;

TextureManager *TextureManager::instance = 0;

TextureManager::TextureManager()
{
	ilInit();
	ilutInit();
	ilutRenderer(ILUT_OPENGL);
	loaded = map<string,GLuint>();
}

TextureManager::~TextureManager()
{
	deleteAllTextures();
	instance = 0;
}

TextureManager* TextureManager::getInstance()
{
	if( !instance ) 
		instance = new TextureManager;
	return instance;
}


GLuint TextureManager::loadTexture( const char* fileName )
{
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
	/*std::wstring wc(sFileName.length()*2, L'#');
	size_t numConverted = 0;
	mbstowcs_s( &numConverted, &wc[0], sFileName.length(), sFileName.c_str(), sFileName.length());*/
	GLuint tex =  ilutGLLoadImage(wc_imgPath);
	
	loaded[ sFileName ] = tex;

	glDisable(GL_TEXTURE_2D);
	return tex;
}

void TextureManager::deleteAllTextures()
{	
	GLuint *textureNames = new GLuint[ loaded.size() ];	
	int index = 0;

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