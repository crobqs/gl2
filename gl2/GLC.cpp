#include "GLC.hpp"

void _GLCheckError(const char* file, int line) {
    GLenum err;
    while ((err = glGetError()) != GL_NO_ERROR) {
        std::cerr << "OpenGL Error [" << err << "] at " << file << ":" << line << std::endl;
    }
}
