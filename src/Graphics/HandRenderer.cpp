#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "HandRenderer.h"
#include "Shader.h"

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <cmath>
#include <cstdio>

static unsigned int LoadTexture(const char* path)
{
    int w, h, ch;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load(path, &w, &h, &ch, 4);

    if (!data) {
        printf("FAILED TO LOAD TEXTURE: %s\n", path);
        return 0;
    }

    unsigned int tex;
    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);

    glTexImage2D(
        GL_TEXTURE_2D, 0, GL_RGBA,
        w, h, 0,
        GL_RGBA, GL_UNSIGNED_BYTE, data
    );

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    stbi_image_free(data);
    return tex;
}

void HandRenderer::Init() {
    shader = new Shader(
        "../../shaders/hand.vs",
        "../../shaders/hand.fs"
    );

    float verts[] = {
        -0.5f, -0.5f,
         0.5f, -0.5f,
         0.5f,  0.5f,
        -0.5f, -0.5f,
         0.5f,  0.5f,
        -0.5f,  0.5f
    };

    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    toolTexture = LoadTexture("../../src/textures/shovel.png");
}

void HandRenderer::SetToolVisible(bool v) {
    toolVisible = v;
}

void HandRenderer::Update(float dt, bool mining) {
    if (mining) {
        anim += dt * 8.f;
        if (anim > 1.f) anim -= 1.f;
    } else {
        anim *= 0.9f; 
    }
}

void HandRenderer::Render() {
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    shader->use();

    float swing = std::sin(anim * 3.14159f);
    float angle = swing * 0.6f;
    float bob   = std::abs(swing) * 0.05f;

    glm::mat4 base(1.f);
    base = glm::translate(base, glm::vec3(0.62f, -0.75f + bob, 0.f));
    base = glm::rotate(base, angle, glm::vec3(0, 0, 1));

    glBindVertexArray(vao);

    if (toolVisible) {
        glm::mat4 m = base;
        m = glm::translate(m, glm::vec3(0.05f, 0.45f, 0.f));
        m = glm::rotate(m, glm::radians(-15.f), glm::vec3(0,0,1));
        m = glm::scale(m, glm::vec3(0.45f, 1.2f, 1.f));

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


    {
        glm::mat4 m = base;
        m = glm::translate(m, glm::vec3(-0.15f, -0.15f, 0.f));
        m = glm::scale(m, glm::vec3(0.2f, 0.8f, 1.f));

        glUniformMatrix4fv(
            glGetUniformLocation(shader->ID, "transform"),
            1, GL_FALSE, &m[0][0]
        );

        glUniform1i(glGetUniformLocation(shader->ID, "mode"), 0);
        glDrawArrays(GL_TRIANGLES, 0, 6);
    }

    glDisable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);
}
