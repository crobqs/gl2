#ifndef GL_ERROR_CHECK_HPP
#define GL_ERROR_CHECK_HPP

#include <iostream>
#include "glad/glad.h"

#define GLC() _GLCheckError(__FILE__, __LINE__)

void _GLCheckError(const char* file, int line);

#endif
