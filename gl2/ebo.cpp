#include "ebo.hpp"

EBO::EBO(void) {
    glCreateBuffers(1, &id); GLC();
}

EBO::EBO(std::vector<float>& vertices) {
    glCreateBuffers(1, &id); GLC();
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id); GLC();
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, vertices.size() * sizeof(unsigned int), vertices.data(), GL_STATIC_DRAW); GLC();
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); GLC();
}

EBO::~EBO(void) {
    glDeleteBuffers(1, &id); GLC();
}

unsigned int EBO::getId(void) const { return id; }

void EBO::bind(void) const {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id); GLC();
}

void EBO::unbind(void) const {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); GLC();
}

void EBO::setData(const void* data, int size) const {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id); GLC();
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, size * sizeof(unsigned int), data, GL_STATIC_DRAW); GLC();
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); GLC();
}