#pragma once
#include <GL/glew.h>

class CubeMap {
public:
	GLuint Handle;
	void LoadFile(const char** Paths);
	void Bind(int index = 0);
};