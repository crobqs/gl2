#ifndef VAO_HPP
#define VAO_HPP

#include <iostream>
#include "glad/glad.h"
#include "GLC.hpp"

class VAO {
private:
    unsigned int id;
public:
    VAO(void);
    ~VAO(void);
    unsigned int getId(void) const;
    void bind(void) const;
    void unbind(void) const;
    void addAttrib(unsigned int vboId, unsigned int index, int size, int stride, int skip) const;
};

#endif /* VAO_HPP */