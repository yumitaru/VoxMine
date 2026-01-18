#include "HandRenderer.h"
#include "Shader.h"

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <cmath>

void HandRenderer::Init()
{
    shader = new Shader(
        "../../shaders/hand.vs",
        "../../shaders/hand.fs"
    );

    // Sześcian (36 vertexów)
    float verts[] = {
        // front
        -0.5f,-0.5f, 0.5f,  0.5f,-0.5f, 0.5f,  0.5f, 0.5f, 0.5f,
        -0.5f,-0.5f, 0.5f,  0.5f, 0.5f, 0.5f, -0.5f, 0.5f, 0.5f,
        // back
        -0.5f,-0.5f,-0.5f, -0.5f, 0.5f,-0.5f,  0.5f, 0.5f,-0.5f,
        -0.5f,-0.5f,-0.5f,  0.5f, 0.5f,-0.5f,  0.5f,-0.5f,-0.5f,
        // left
        -0.5f,-0.5f,-0.5f, -0.5f,-0.5f, 0.5f, -0.5f, 0.5f, 0.5f,
        -0.5f,-0.5f,-0.5f, -0.5f, 0.5f, 0.5f, -0.5f, 0.5f,-0.5f,
        // right
         0.5f,-0.5f,-0.5f,  0.5f, 0.5f,-0.5f,  0.5f, 0.5f, 0.5f,
         0.5f,-0.5f,-0.5f,  0.5f, 0.5f, 0.5f,  0.5f,-0.5f, 0.5f,
        // top
        -0.5f, 0.5f,-0.5f, -0.5f, 0.5f, 0.5f,  0.5f, 0.5f, 0.5f,
        -0.5f, 0.5f,-0.5f,  0.5f, 0.5f, 0.5f,  0.5f, 0.5f,-0.5f,
        // bottom
        -0.5f,-0.5f,-0.5f,  0.5f,-0.5f,-0.5f,  0.5f,-0.5f, 0.5f,
        -0.5f,-0.5f,-0.5f,  0.5f,-0.5f, 0.5f, -0.5f,-0.5f, 0.5f
    };

    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
}

void HandRenderer::SetToolVisible(bool v)
{
    toolVisible = v;
}

void HandRenderer::Update(float dt, bool mining)
{
    if (mining) {
        anim += dt * 8.f;
        if (anim > 1.f) anim -= 1.f;
    } else {
        anim *= 0.9f;
    }
}

void HandRenderer::DrawCube(const glm::mat4& model, int mode,
                            const glm::mat4& view,
                            const glm::mat4& proj)
{
    glm::mat4 mvp = proj * view * model;
    glUniformMatrix4fv(
        glGetUniformLocation(shader->ID, "MVP"),
        1, GL_FALSE, &mvp[0][0]
    );
    glUniform1i(glGetUniformLocation(shader->ID, "mode"), mode);
    glDrawArrays(GL_TRIANGLES, 0, 36);
}

void HandRenderer::Render()
{
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);

    shader->use();
    glBindVertexArray(vao);

    float swing = std::sin(anim * 3.14159f);
    float angle = swing * 0.6f;
    float bob   = std::abs(swing) * 0.05f;

    glm::mat4 proj = glm::perspective(
        glm::radians(60.f), 16.f / 9.f, 0.01f, 10.f
    );
    glm::mat4 view = glm::translate(glm::mat4(1.f), glm::vec3(0, 0, -2.5f));

    glm::mat4 base(1.f);
    base = glm::translate(base, glm::vec3(0.9f, -0.9f + bob, 0.f));
    base = glm::rotate(base, angle, glm::vec3(0, 0, 1));

    // =========================
    // ŁOPATA 3D
    // =========================
    if (toolVisible)
    {
        // Trzonek
        glm::mat4 stick = base;
        stick = glm::translate(stick, glm::vec3(0.f, 0.25f, 0.f));
        stick = glm::rotate(stick, glm::radians(-45.f), glm::vec3(0, 0, 1));
        stick = glm::scale(stick, glm::vec3(0.07f, 0.8f, 0.07f));
        
        DrawCube(stick, 1, view, proj);

        // Głowica
        glm::mat4 head = base;
        head = glm::translate(head, glm::vec3(0.3f, 0.5f, 0.f));
        head = glm::rotate(head, glm::radians(-45.f), glm::vec3(0, 0, 1));
        head = glm::scale(head, glm::vec3(0.35f, 0.25f, 0.15f));
        
        DrawCube(head, 2, view, proj);
    }

    // =========================
    // RĘKA
    // =========================
    glm::mat4 hand = base;
    hand = glm::translate(hand, glm::vec3(-0.15f, -0.2f, 0.f));
    hand = glm::scale(hand, glm::vec3(0.18f, 0.5f, 0.18f));
    DrawCube(hand, 0, view, proj);
    glEnable(GL_DEPTH_TEST);
}
