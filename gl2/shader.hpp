#ifndef SHADER_HPP
#define SHADER_HPP

#include <iostream>
#include <string>
#include <unordered_map>
#include <fstream>
#include <sstream>
#include "glad/glad.h"
#include <glm/glm.hpp>
#include "GLC.hpp"

class Shader {
private:
    unsigned int id;
    unsigned int vaoId;
    std::string filename;
    std::unordered_map<std::string, std::string> shaderSrc;
    bool isGeometryShaderPresent;

public:
    Shader(const std::string filename_);
    ~Shader(void);
    unsigned int getId(void) const;
    void setVao(unsigned int vaoId_);
    void bind(void) const;
    void unbind(void) const;
    void setBool(const std::string& name, bool value) const;
    void setInt(const std::string& name, int value) const;
    void setFloat(const std::string& name, float value) const;
    void setVec2(const std::string& name, const glm::vec2& value) const;
    void setVec2(const std::string& name, float x, float y) const;
    void setVec3(const std::string& name, const glm::vec3& value) const;
    void setVec3(const std::string& name, float x, float y, float z) const;
    void setVec4(const std::string& name, const glm::vec4& value) const;
    void setVec4(const std::string& name, float x, float y, float z, float w) const;
    void setMat2(const std::string& name, const glm::mat2& value) const;
    void setMat3(const std::string& name, const glm::mat3& value) const;
    void setMat4(const std::string& name, const glm::mat4& value) const;

private:
    void parseShader(void);
    void compileShader(void);
    void checkCompileErrors(unsigned int id_, const std::string type) const;
};

#endif /* SHADER_HPP */