#include "HUDRenderer.h"
#include "Shader.h"

#include <glad/glad.h>

void HUDRenderer::Init() {
    shader = new Shader(
        "../../shaders/hud.vs",
        "../../shaders/hud.fs"
    );

    float verts[] = {
        -0.02f,  0.0f,
         0.02f,  0.0f,
         0.0f,  -0.02f,
         0.0f,   0.02f
    };

    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
}

void HUDRenderer::Render() {
    glDisable(GL_DEPTH_TEST);

    shader->use();

    int colorLoc = glGetUniformLocation(shader->ID, "color");
    glUniform3f(colorLoc, 1.0f, 1.0f, 1.0f);

    glBindVertexArray(vao);
    glDrawArrays(GL_LINES, 0, 4);

    glEnable(GL_DEPTH_TEST);
}
