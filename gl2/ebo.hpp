#ifndef EBO_HPP
#define EBO_HPP

#include <vector>
#include "glad/glad.h"
#include "GLC.hpp"

class EBO {
private:
    unsigned int id;
public:
    EBO(void);
    EBO(std::vector<float>& vertices);
    ~EBO(void);
    unsigned int getId(void) const;
    void bind(void) const;
    void unbind(void) const;
    void setData(const void* data, int size) const;
};

#endif /* EBO_HPP */