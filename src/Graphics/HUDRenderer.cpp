#include "HUDRenderer.h"
#include "Shader.h"

#include <glad/glad.h>

void HUDRenderer::Init()
{
    shader = new Shader(
        "../../shaders/hud.vs",
        "../../shaders/hud.fs"
    );

    float dummyLine[8] = { 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f };

    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
     glBufferData(
        GL_ARRAY_BUFFER,
        sizeof(float) * 12,
        nullptr,
        GL_DYNAMIC_DRAW
    ); 

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    float quad[] = {
        -0.6f,  0.1f,
         0.6f,  0.1f,
         0.6f, -0.1f,

        -0.6f,  0.1f,
         0.6f, -0.1f,
        -0.6f, -0.1f
    };

    glGenVertexArrays(1, &quadVAO);
    glGenBuffers(1, &quadVBO);

    glBindVertexArray(quadVAO);
    glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quad), quad, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    
}

static void DrawQuad(GLuint vbo, float x, float y, float w, float h)
{
    float verts[] = {
        x - w, y - h,
        x + w, y - h,
        x + w, y + h,

        x - w, y - h,
        x + w, y + h,
        x - w, y + h
    };

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(verts), verts);
    glDrawArrays(GL_TRIANGLES, 0, 6);

}

void HUDRenderer::Render()
{
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE); 

    shader->use();
    glUniform3f(
        glGetUniformLocation(shader->ID, "color"),
        1.f, 1.f, 1.f
    );

    glBindVertexArray(vao);

    float size = 0.015f;
    float thick = 0.003f;

    DrawQuad(vbo, 0.f, 0.f, size, thick); 

    DrawQuad(vbo, 0.f, 0.f, thick, size);

    glEnable(GL_DEPTH_TEST);

}

static void DrawSegment(GLuint vbo, float x1, float y1, float x2, float y2)
{
    float verts[] = { x1, y1, x2, y2 };
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(verts), verts);
    glDrawArrays(GL_LINES, 0, 2);

}

void HUDRenderer::DrawLetter(float x, float y, float s, int type)
{
    glBindVertexArray(vao);

    float h = s * 1.4f;
    float w = s;

    switch (type)
    {
        case 0: 
            DrawSegment(vbo, x, y, x, y - h);
            DrawSegment(vbo, x, y - h, x + w * 0.5f, y - h * 0.6f);
            DrawSegment(vbo, x + w * 0.5f, y - h * 0.6f, x + w, y - h);
            DrawSegment(vbo, x + w, y - h, x + w, y);
            break;

        case 1: 
            DrawSegment(vbo, x, y, x + w * 0.5f, y - h * 0.5f);
            DrawSegment(vbo, x + w, y, x + w * 0.5f, y - h * 0.5f);
            DrawSegment(vbo, x + w * 0.5f, y - h * 0.5f, x + w * 0.5f, y - h);
            break;

        case 2: 
            DrawSegment(vbo, x + w, y, x, y);
            DrawSegment(vbo, x, y, x, y - h);
            DrawSegment(vbo, x, y - h, x + w, y - h);
            DrawSegment(vbo, x + w, y - h, x + w, y - h * 0.6f);
            DrawSegment(vbo, x + w * 0.5f, y - h * 0.6f, x + w, y - h * 0.6f);
            break;

        case 3: 
            DrawSegment(vbo, x, y, x, y - h);
            DrawSegment(vbo, x, y, x + w, y);
            DrawSegment(vbo, x + w, y, x + w, y - h * 0.5f);
            DrawSegment(vbo, x + w, y - h * 0.5f, x, y - h * 0.5f);
            DrawSegment(vbo, x, y - h * 0.5f, x + w, y - h);
            break;

        case 4: 
            DrawSegment(vbo, x, y - h, x + w * 0.5f, y);
            DrawSegment(vbo, x + w * 0.5f, y, x + w, y - h);
            DrawSegment(vbo, x + w * 0.25f, y - h * 0.5f, x + w * 0.75f, y - h * 0.5f);
            break;

        case 5:
            DrawSegment(vbo, x, y, x, y - h);
            DrawSegment(vbo, x, y, x + w, y - h);
            DrawSegment(vbo, x + w, y - h, x + w, y);
            break;
    }

}

void HUDRenderer::RenderGameWon()
{
    glDisable(GL_DEPTH_TEST);
    shader->use();

    glUniform3f(glGetUniformLocation(shader->ID, "color"), 1.f, 0.85f, 0.2f);
    glBindVertexArray(quadVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    glUniform3f(glGetUniformLocation(shader->ID, "color"), 1.f, 1.f, 1.f);

    float s = 0.07f;
    float y = 0.05f;
    float x = -0.35f;
    float d = 0.1f;

    DrawLetter(x + d * 0, y, s, 0); 
    DrawLetter(x + d * 1, y, s, 1); 
    DrawLetter(x + d * 2, y, s, 2); 
    DrawLetter(x + d * 3, y, s, 3); 
    DrawLetter(x + d * 4, y, s, 4); 
    DrawLetter(x + d * 5, y, s, 5); 
    DrawLetter(x + d * 6, y, s, 4); 

    glEnable(GL_DEPTH_TEST);

}
