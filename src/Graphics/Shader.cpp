#include "Shader.h"
#include <glad/glad.h>
#include <fstream>
#include <sstream>
#include <iostream>

Shader::Shader(const char* vs, const char* fs) {
    std::ifstream v(vs), f(fs);

    if (!v.is_open()) {
        std::cout << "Cannot open vertex shader file: " << vs << std::endl;
    }

    if (!f.is_open()) {
        std::cout << "Cannot open fragment shader file: " << fs << std::endl;
    }

    std::stringstream vsr, fsr;
    vsr << v.rdbuf();
    fsr << f.rdbuf();

    std::string vc = vsr.str();
    std::string fc = fsr.str();
    const char* vsc = vc.c_str();
    const char* fsc = fc.c_str();

    unsigned int vsh = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vsh, 1, &vsc, nullptr);
    glCompileShader(vsh);

    int success;
    glGetShaderiv(vsh, GL_COMPILE_STATUS, &success);

    if (!success) {
        char infoLog[512];
        glGetShaderInfoLog(vsh, 512, nullptr, infoLog);
        std::cout << "Vertex shader compilation failed: " << infoLog << std::endl;
    }

    unsigned int fsh = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fsh, 1, &fsc, nullptr);
    glCompileShader(fsh);

    glGetShaderiv(fsh, GL_COMPILE_STATUS, &success);

    if (!success) {
        char infoLog[512];
        glGetShaderInfoLog(fsh, 512, nullptr, infoLog);
        std::cout << "Fragment shader compilation failed: " << infoLog << std::endl;
    }

    ID = glCreateProgram();
    glAttachShader(ID, vsh);
    glAttachShader(ID, fsh);
    glLinkProgram(ID);

    glGetProgramiv(ID, GL_LINK_STATUS, &success);

    if (!success) {
        char infoLog[512];
        glGetProgramInfoLog(ID, 512, nullptr, infoLog);
        std::cout << "Program linking failed: " << infoLog << std::endl;
    }

    glDeleteShader(vsh);
    glDeleteShader(fsh);
}

void Shader::use() const {
    glUseProgram(ID);
}

void Shader::setMat4(const std::string& n, const glm::mat4& m) const {
    glUniformMatrix4fv(
        glGetUniformLocation(ID, n.c_str()),
        1,
        GL_FALSE,
        &m[0][0]
    );
}

void Shader::setInt(const std::string& n, int v) const {
    glUniform1i(glGetUniformLocation(ID, n.c_str()), v);
}

void Shader::setVec3(const std::string& n, float x, float y, float z) const {
    glUniform3f(glGetUniformLocation(ID, n.c_str()), x, y, z);
}
