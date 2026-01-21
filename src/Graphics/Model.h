// Model.h
#pragma once
#include <string>

class Model {
    unsigned int vao, vbo;
    unsigned int texture;
    int vertexCount;
    unsigned int ebo;
    unsigned int indexCount;
public:
    Model(const std::string& path);
    void Draw();
    unsigned int GetTextureID() const;


};
