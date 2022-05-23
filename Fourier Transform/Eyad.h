#pragma once

#include <glad/glad.h>

#define ASSERT(x) if (!(x)) __debugbreak();
#define call(x) GLClearError();\
			    x;\
				ASSERT(GLLogCall(#x, __FILE__, __LINE__))

void GLClearError();
bool GLLogCall(const char* func, const char* file, int line);