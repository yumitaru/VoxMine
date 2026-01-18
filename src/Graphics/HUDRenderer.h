#pragma once

class Shader;

class HUDRenderer {
public:
    void Init();
    void Render();
    void RenderGameWon();


private:
    void DrawLetter(float x, float y, float s, int type);

    unsigned int vao = 0;
    unsigned int vbo = 0;

    unsigned int quadVAO = 0; 
    unsigned int quadVBO = 0;

    Shader* shader = nullptr;
};
