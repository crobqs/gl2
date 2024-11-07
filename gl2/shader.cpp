#include "shader.hpp"

Shader::Shader(const std::string filename_) : id(0), vaoId(0), isGeometryShaderPresent(false) {
    filename = filename_;
    parseShader();
    compileShader();
}

Shader::~Shader(void) {
    glDeleteProgram(id); GLC();
}

unsigned int Shader::getId(void) const { return id; }

void Shader::setVao(unsigned int vaoId_) {
    vaoId = vaoId_;
}

void Shader::bind(void) const {
    glBindVertexArray(vaoId); GLC();
    glUseProgram(id); GLC();
}

void Shader::unbind(void) const {
    glUseProgram(0); GLC();
    glBindVertexArray(0); GLC();
}

void Shader::parseShader(void) {
    enum class Shadertype { VERTEX = 0, FRAGMENT, GEOMETRY, NONE };
    Shadertype type = Shadertype::NONE;
    std::stringstream ss[3];
    std::ifstream stream(filename);
    std::string line;
    if (!stream.is_open()) {
        std::cerr << "Failed to open file: \"" << filename << "\"" << "\n";
        std::exit(1);
    }
    while (std::getline(stream, line)) {
        if (line.find("#shader") != std::string::npos) {
            if (line.find("vertex") != std::string::npos)
                type = Shadertype::VERTEX;
            else if (line.find("fragment") != std::string::npos)
                type = Shadertype::FRAGMENT;
            else if (line.find("geometry") != std::string::npos) {
                type = Shadertype::GEOMETRY;
                isGeometryShaderPresent = true;
            }
        }
        else if (type != Shadertype::NONE) {
            ss[static_cast<int>(type)] << line << "\n";
        }
    }
    shaderSrc["VERTEX"] = ss[static_cast<int>(Shadertype::VERTEX)].str();
    shaderSrc["FRAGMENT"] = ss[static_cast<int>(Shadertype::FRAGMENT)].str();
    if (isGeometryShaderPresent)
        shaderSrc["GEOMETRY"] = ss[static_cast<int>(Shadertype::GEOMETRY)].str();
}

void Shader::compileShader(void) {
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER); GLC();
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER); GLC();
    unsigned int geometryShader;
    if (isGeometryShaderPresent) {
        geometryShader = glCreateShader(GL_GEOMETRY_SHADER); GLC();
    }

    const char* vertexShaderSrc = shaderSrc["VERTEX"].c_str();
    const char* fragmentShaderSrc = shaderSrc["FRAGMENT"].c_str();
    const char* geometryShaderSrc;
    if (isGeometryShaderPresent) {
        geometryShaderSrc = shaderSrc["GEOMETRY"].c_str();
    }

    glShaderSource(vertexShader, 1, &vertexShaderSrc, nullptr); GLC();
    glShaderSource(fragmentShader, 1, &fragmentShaderSrc, nullptr); GLC();
    if (isGeometryShaderPresent) {
        glShaderSource(geometryShader, 1, &geometryShaderSrc, nullptr); GLC();
    }

    glCompileShader(vertexShader); GLC();
    checkCompileErrors(vertexShader, "VERTEX");
    glCompileShader(fragmentShader); GLC();
    checkCompileErrors(fragmentShader, "FRAGMENT");
    if (isGeometryShaderPresent) {
        glCompileShader(geometryShader); GLC();
        checkCompileErrors(geometryShader, "GEOMETRY");
    }

    id = glCreateProgram(); GLC();
    glAttachShader(id, vertexShader); GLC();
    glAttachShader(id, fragmentShader); GLC();
    if (isGeometryShaderPresent) {
        glAttachShader(id, geometryShader); GLC();
    }
    glLinkProgram(id); GLC();
    checkCompileErrors(id, "PROGRAM");

    shaderSrc.clear();
    glDeleteShader(vertexShader); GLC();
    glDeleteShader(fragmentShader); GLC();
    if (isGeometryShaderPresent) {
        glDeleteShader(geometryShader); GLC();
    }
}

void Shader::checkCompileErrors(unsigned int id_, const std::string type) const {
    int success;
    char infoLog[1024];
    if (type != "PROGRAM") {
        glGetShaderiv(id_, GL_COMPILE_STATUS, &success); GLC();
        if (!success) {
            glGetShaderInfoLog(id_, 1024, nullptr, infoLog); GLC();
            std::cerr << "[\"" << filename << "\"] "
                << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n"
                << infoLog << "\n -- ------- --" << std::endl;
            exit(1);
        }
    }
    else {
        glGetProgramiv(id_, GL_LINK_STATUS, &success); GLC();
        if (!success) {
            glGetProgramInfoLog(id_, 1024, nullptr, infoLog); GLC();
            std::cerr << "[\"" << filename << "\"] "
                << "ERROR::PROGRAM_LINKING_ERROR of type" << type << "\n"
                << infoLog << "\n -- ------- --" << std::endl;
            exit(1);
        }
    }
}

void Shader::setBool(const std::string& name, bool value) const {
    glUniform1i(glGetUniformLocation(id, name.c_str()), static_cast<int>(value)); GLC();
}

void Shader::setInt(const std::string& name, int value) const {
    glUniform1i(glGetUniformLocation(id, name.c_str()), value); GLC();
}

void Shader::setFloat(const std::string& name, float value) const {
    glUniform1f(glGetUniformLocation(id, name.c_str()), value); GLC();
}

void Shader::setVec2(const std::string& name, const glm::vec2& value) const {
    glUniform2fv(glGetUniformLocation(id, name.c_str()), 1, &value[0]); GLC();
}

void Shader::setVec2(const std::string& name, float x, float y) const {
    glUniform2f(glGetUniformLocation(id, name.c_str()), x, y); GLC();
}

void Shader::setVec3(const std::string& name, const glm::vec3& value) const {
    glUniform3fv(glGetUniformLocation(id, name.c_str()), 1, &value[0]); GLC();
}

void Shader::setVec3(const std::string& name, float x, float y, float z) const {
    glUniform3f(glGetUniformLocation(id, name.c_str()), x, y, z); GLC();
}

void Shader::setVec4(const std::string& name, const glm::vec4& value) const {
    glUniform4fv(glGetUniformLocation(id, name.c_str()), 1, &value[0]); GLC();
}

void Shader::setVec4(const std::string& name, float x, float y, float z, float w) const {
    glUniform4f(glGetUniformLocation(id, name.c_str()), x, y, z, w); GLC();
}

void Shader::setMat2(const std::string& name, const glm::mat2& value) const {
    glUniformMatrix2fv(glGetUniformLocation(id, name.c_str()), 1, GL_FALSE, &value[0][0]); GLC();
}

void Shader::setMat3(const std::string& name, const glm::mat3& value) const {
    glUniformMatrix3fv(glGetUniformLocation(id, name.c_str()), 1, GL_FALSE, &value[0][0]); GLC();
}

void Shader::setMat4(const std::string& name, const glm::mat4& value) const {
    glUniformMatrix4fv(glGetUniformLocation(id, name.c_str()), 1, GL_FALSE, &value[0][0]); GLC();
}