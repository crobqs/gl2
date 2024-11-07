#include "vbo.hpp"

VBO::VBO(void) {
    glCreateBuffers(1, &id); GLC();
}

VBO::VBO(std::vector<float>& vertices) {
    glCreateBuffers(1, &id); GLC();
    glBindBuffer(GL_ARRAY_BUFFER, id); GLC();
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW); GLC();
    glBindBuffer(GL_ARRAY_BUFFER, 0); GLC();
}

VBO::~VBO(void) {
    glDeleteBuffers(1, &id); GLC();
}

unsigned int VBO::getId(void) const { return id; }

void VBO::bind(void) const {
    glBindBuffer(GL_ARRAY_BUFFER, id); GLC();
}

void VBO::unbind(void) const {
    glBindBuffer(GL_ARRAY_BUFFER, 0); GLC();
}

void VBO::setData(const void* data, int size) const {
    glBindBuffer(GL_ARRAY_BUFFER, id); GLC();
    glBufferData(GL_ARRAY_BUFFER, size * sizeof(float), data, GL_STATIC_DRAW); GLC();
    glBindBuffer(GL_ARRAY_BUFFER, 0); GLC();
}