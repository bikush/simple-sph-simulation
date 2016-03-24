#pragma once
#ifndef _TEXMAN_H
#define _TEXMAN_H

#include <stdlib.h>
#include <map>
#include <string>
#include <GL/glew.h>

class TextureManager
{
private:
	std::map< std::string, GLuint > loaded;
	bool isInitialized;
	void deleteAllTextures();

public:
	static TextureManager instance;

	TextureManager();
	~TextureManager();

	GLuint loadTexture( const char* fileName );
};

#endif