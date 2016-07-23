
#include "TextureManager.h"
#include "lodepng.h"
#include "Utility.h"

using namespace std;

TextureManager TextureManager::instance;

TextureManager::TextureManager()
{
}

TextureManager::~TextureManager()
{
	deleteAllTextures();
}

GLuint TextureManager::loadTexture( const char* fileName )
{
	string sFileName = string(fileName);
	map<string, GLuint>::iterator found = loaded.find(sFileName);

	if (found != loaded.end())
	{
		return found->second;
	}
	
	// Load file and decode image.
	std::vector<unsigned char> image;
	unsigned width;
	unsigned height;
	unsigned error = lodepng::decode(image, width, height, sFileName);
	
	// Ignore png loading errors.
	if (error != 0)
	{
		return 0;
	}

	// Texture size must be power of two for the primitive OpenGL version this is written for. Find next power of two.
	size_t u2 = nextPOT(width);
	size_t v2 = nextPOT(height);
	// Ratio for power of two version compared to actual version, to render the non power of two image with proper size.
	double u3 = (double)width / u2;
	double v3 = (double)height / v2;

	// Make power of two version of the image.
	std::vector<unsigned char> image_POT(u2 * v2 * 4);
	for (size_t y = 0; y < height; y++)
	{
		for (size_t x = 0; x < width; x++)
		{
			for (size_t c = 0; c < 4; c++)
			{
				image_POT[4 * u2 * y + 4 * x + c] = image[4 * width * y + 4 * x + c];
			}
		}
	}

	// Enable the texture for OpenGL.
	glEnable(GL_TEXTURE_2D);

	GLuint glTextureId;
	glGenTextures(1, &glTextureId);
	glBindTexture(GL_TEXTURE_2D, glTextureId);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); //GL_NEAREST = no smoothing
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, 4, u2, v2, 0, GL_RGBA, GL_UNSIGNED_BYTE, &image_POT[0]);
		
	loaded[ sFileName ] = glTextureId;

	glDisable(GL_TEXTURE_2D);
	return glTextureId;
}

void TextureManager::deleteAllTextures()
{	
	int texCount = loaded.size();
	GLuint *textureNames = new GLuint[ texCount ];	
	int index = 0;

	for (auto pair : loaded) {
		textureNames[index] = pair.second;
		index++;
		if (index >= texCount) {
			break;
		}
	}

	glDeleteTextures( index, textureNames );
	delete [] textureNames;
	loaded.clear();
}