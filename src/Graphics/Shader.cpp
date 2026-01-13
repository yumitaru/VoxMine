#include "Shader.h"
#include <glad/glad.h>
#include <fstream>
#include <sstream>

Shader::Shader(const char* vs, const char* fs) {
    std::ifstream v(vs), f(fs);
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

    unsigned int fsh = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fsh, 1, &fsc, nullptr);
    glCompileShader(fsh);

    ID = glCreateProgram();
    glAttachShader(ID, vsh);
    glAttachShader(ID, fsh);
    glLinkProgram(ID);

    glDeleteShader(vsh);
    glDeleteShader(fsh);
}

void Shader::use() const {
    glUseProgram(ID);
}

void Shader::setMat4(const std::string& n, const glm::mat4& m) const {
    glUniformMatrix4fv(glGetUniformLocation(ID, n.c_str()), 1, GL_FALSE, &m[0][0]);
}
