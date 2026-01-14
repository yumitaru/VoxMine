#pragma once

class Shader;

class HandRenderer {
public:
    void Init();
    void Update(float dt, bool mining);
    void SetToolVisible(bool v);
    void Render();

private:
    unsigned int vao = 0;
    unsigned int vbo = 0;
    unsigned int toolTexture = 0;

    Shader* shader = nullptr;

    float anim = 0.f;

    bool toolVisible = true;
};
