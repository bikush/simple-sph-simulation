#pragma once
#ifndef _TEXMAN_H
#define _TEXMAN_H

#include <stdlib.h>
#include <map>
#include <string>
#include <GL/glew.h>

class TextureManager
{
	TextureManager();	

	std::map< std::string, GLuint > loaded;
	static TextureManager* instance;

	void deleteAllTextures();

public:
	~TextureManager();

	static TextureManager* getInstance();
	GLuint loadTexture( const char* fileName );
};

#endif