#include "vao.hpp"

VAO::VAO(void) {
    glCreateVertexArrays(1, &id); GLC();
}

VAO::~VAO(void) {
    glDeleteVertexArrays(1, &id); GLC();
}

unsigned int VAO::getId(void) const { return id; }

void VAO::bind(void) const {
    glBindVertexArray(id); GLC();
}

void VAO::unbind(void) const {
    glBindVertexArray(0); GLC();
}

void VAO::addAttrib(unsigned int vboId, unsigned int index, int size, int stride, int skip) const {
    glBindVertexArray(id); GLC();
    glBindBuffer(GL_ARRAY_BUFFER, vboId); GLC();
    glVertexAttribPointer(index, size, GL_FLOAT, GL_FALSE, stride * sizeof(float), (void*)(skip * sizeof(float))); GLC();
    glEnableVertexAttribArray(index); GLC();
    glBindVertexArray(0); GLC();
    glBindBuffer(GL_ARRAY_BUFFER, 0); GLC();
}