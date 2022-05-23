#include "Eyad.h"
#include <iostream>

void GLClearError() {
	while (glGetError() != GL_NO_ERROR);
}

bool GLLogCall(const char* func, const char* file, int line) {
	while (GLenum error = glGetError()) {
		std::cout << "(OpenGL Error) [" << error << "]" << std::endl << "[" << file << "]" << std::endl << "Function " << func << " triggered an error on line " << line << std::endl;
		return false;
	}
	return true;
}

