#pragma once

#include <glm/glm.hpp>

class Shader;

class HandRenderer {
public:
    void Init();
    void Update(float dt, bool mining);
    void SetToolVisible(bool v);
    void DrawCube(const glm::mat4& model, int mode,
                            const glm::mat4& view,
                            const glm::mat4& proj);
    void Render();
    void DrawLetter(float x, float y, float s, int type);

private:
    unsigned int vao = 0;
    unsigned int vbo = 0;
    unsigned int toolTexture = 0;

    Shader* shader = nullptr;

    float anim = 0.f;

    bool toolVisible = true;
};
