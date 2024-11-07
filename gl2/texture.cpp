#include "texture.hpp"

Texture::Texture(const std::string& filename, int textureUnit_) :
    textureUnit(textureUnit_)
{
    glGenTextures(1, &id); GLC();
    glBindTexture(GL_TEXTURE_2D, id); GLC();

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); GLC();
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); GLC();
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); GLC();
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); GLC();

    stbi_set_flip_vertically_on_load(true);
    loadFromFile(filename);

    glBindTexture(GL_TEXTURE_2D, 0); GLC();
}

Texture::~Texture(void) {
    glDeleteTextures(1, &id); GLC();
}

unsigned int Texture::getId(void) const { return id; }

void Texture::bind(void) const {
    glActiveTexture(textureUnit); GLC();
    glBindTexture(GL_TEXTURE_2D, id); GLC();
}

void Texture::unbind(void) const {
    glBindTexture(GL_TEXTURE_2D, 0); GLC();
}

void Texture::loadFromFile(const std::string& filename) {
    int width;
    int height;
    int channels;
    unsigned char* data = stbi_load(filename.c_str(), &width, &height, &channels, 0);
    if (data) {
        int format = ((channels == 4) ? GL_RGBA : GL_RGB);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data); GLC();
        glGenerateMipmap(GL_TEXTURE_2D); GLC();
    }
    else {
        std::cerr << "Failed to load texture: " << filename << std::endl;
    }
    stbi_image_free(data);
}