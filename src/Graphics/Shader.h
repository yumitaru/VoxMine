#pragma once
#include <string>
#include <glm/glm.hpp>

class Shader {
public:
    unsigned int ID;

    Shader(const char* vs, const char* fs);
    void use() const;
    void setMat4(const std::string& name, const glm::mat4& m) const;    
    void setInt(const std::string& name, int value) const;
    void setVec3(const std::string& n, float x, float y, float z) const;
};
