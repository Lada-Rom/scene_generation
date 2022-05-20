#ifndef TEXTURE_TEXTURE_H_20222005
#define TEXTURE_TEXTURE_H_20222005

#include <string>
#include <GL/freeglut.h>

struct Texture {
	unsigned int width_{ };
	unsigned int height_{ };
	int comp_{ };
	GLuint id_{ };
	std::string filename{ };
};

#endif
