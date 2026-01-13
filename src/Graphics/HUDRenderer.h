#pragma once

class Shader;

class HUDRenderer {
public:
    void Init();
    void Render();

private:
    unsigned int vao = 0;
    unsigned int vbo = 0;
    Shader* shader = nullptr;
};
