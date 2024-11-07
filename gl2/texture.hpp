#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include <iostream>
#include <string>
#include "glad/glad.h"
#include "stb/stb_image.h"
#include "GLC.hpp"

class Texture {
private:
    unsigned int id;
    int textureUnit;
public:
    Texture(const std::string& filename, int textureUnit_);
    Texture(const Texture&) = delete;
    Texture& operator=(Texture&) = delete;
    ~Texture(void);
    unsigned int getId(void) const;
    void bind(void) const;
    void unbind(void) const;
private:
    void loadFromFile(const std::string& filename);
};

#endif /* TEXTURE_HPP */