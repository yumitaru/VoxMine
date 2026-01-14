#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "HandRenderer.h"
#include "Shader.h"

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <cmath>
#include <cstdio>

// ================= TEXTURE LOADER =================
static unsigned int LoadTexture(const char* path)
{
    int w, h, ch;
    unsigned char* data = stbi_load(path, &w, &h, &ch, 4);
    if (!data) {
        printf("FAILED TO LOAD TEXTURE: %s\n", path);
        return 0;
    }

    unsigned int tex;
    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, data);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    stbi_image_free(data);
    return tex;
}

// ================= INIT =================
void HandRenderer::Init()
{
    shader = new Shader("../../shaders/hand.vs",
                        "../../shaders/hand.fs");

    // POS (x,y) + UV (u,v)
    float verts[] = {
        // pos        // uv
        -0.15f, -0.35f,  0.0f, 0.0f,
         0.15f, -0.35f,  1.0f, 0.0f,
         0.15f,  0.35f,  1.0f, 1.0f,

        -0.15f, -0.35f,  0.0f, 0.0f,
         0.15f,  0.35f,  1.0f, 1.0f,
        -0.15f,  0.35f,  0.0f, 1.0f
    };

    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);

    // position
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE,
                          4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // uv
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE,
                          4 * sizeof(float),
                          (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    toolTexture = LoadTexture("textures/shovel.png");
}

// ================= UPDATE =================
void HandRenderer::Update(float dt, bool mining)
{
    if (mining) {
        anim += dt * 8.f;
        if (anim > 1.f) anim -= 1.f;
    } else {
        anim = 0.f;
    }
}

// ================= RENDER =================
void HandRenderer::Render()
{
    glDisable(GL_DEPTH_TEST);
    shader->use();

    float swing = std::sin(anim * 3.14159f);
    float bob   = std::abs(swing) * 0.06f;

    glm::mat4 base(1.f);
    base = glm::translate(base, glm::vec3(0.55f, -0.55f + bob, 0.f));
    base = glm::rotate(base, swing * 0.4f, glm::vec3(0, 0, 1));

    glBindVertexArray(vao);

    // ===== ŁOPATA =====
    {
        glm::mat4 m = base;
        m = glm::translate(m, glm::vec3(0.20f, 0.32f, 0.f));
        m = glm::rotate(m, glm::pi<float>(), glm::vec3(0, 0, 1)); 
        m = glm::rotate(m, -0.8f, glm::vec3(0, 0, 1));
        m = glm::scale(m, glm::vec3(0.75f, 1.25f, 1.f));

        glUniformMatrix4fv(
            glGetUniformLocation(shader->ID, "transform"),
            1, GL_FALSE, &m[0][0]
        );

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, toolTexture);
        glUniform1i(glGetUniformLocation(shader->ID, "tex"), 0);
        glUniform1i(glGetUniformLocation(shader->ID, "mode"), 1);

        glDrawArrays(GL_TRIANGLES, 0, 6);
    }

    // ===== RĘKA =====
    {
        glm::mat4 m = base;
        m = glm::translate(m, glm::vec3(0.0f, -0.10f, 0.f));
        m = glm::scale(m, glm::vec3(1.0f, 1.15f, 1.f));

        glUniformMatrix4fv(
            glGetUniformLocation(shader->ID, "transform"),
            1, GL_FALSE, &m[0][0]
        );

        glUniform1i(glGetUniformLocation(shader->ID, "mode"), 0);
        glDrawArrays(GL_TRIANGLES, 0, 6);
    }

    glEnable(GL_DEPTH_TEST);
}
