#ifndef TEXTURES_H
#define TEXTURES_H


#include <GL/glew.h>

class Texture2D
{
public:
	Texture2D(const char* path, GLenum wrapMode);
	~Texture2D();

	void enableMipmap();

	void use(GLuint unit = 0);
	static void unuse();

private:
	GLuint m_id;
};


class TextureCubeMap
{
public:
	TextureCubeMap(const char** pathes);
	~TextureCubeMap();

	void use();

private:
	GLuint m_id;
};


#endif // TEXTURES
