#include "HandRenderer.h"
#include "Shader.h"

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <cmath>

void HandRenderer::Init() {
    shader = new Shader(
        "../../shaders/hand.vs",
        "../../shaders/hand.fs"
    );

    float verts[] = {
        -0.15f, -0.35f,
         0.15f, -0.35f,
         0.20f,  0.30f,

        -0.15f, -0.35f,
         0.20f,  0.30f,
        -0.10f,  0.30f
    };

    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
}

void HandRenderer::Update(float dt, bool mining) {
    if (mining) {
        anim += dt * 8.f; 
        if (anim > 1.f) anim -= 1.f;
    } else {
        anim = 0.f;
    }
}

void HandRenderer::Render() {
    glDisable(GL_DEPTH_TEST);
    shader->use();

    float swing = std::sin(anim * 3.14159f);
    float angle = swing * 0.9f;
    float bob   = std::abs(swing) * 0.08f;

    glm::mat4 m(1.f);

    m = glm::translate(m, glm::vec3(0.75f, -0.75f + bob, 0.f));

    m = glm::rotate(m, angle, glm::vec3(0, 0, 1));

    m = glm::scale(m, glm::vec3(
        1.4f,
        1.6f - std::abs(swing) * 0.15f,
        1.f
    ));

    glUniformMatrix4fv(
        glGetUniformLocation(shader->ID, "transform"),
        1, GL_FALSE, &m[0][0]
    );

    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    glEnable(GL_DEPTH_TEST);
}
