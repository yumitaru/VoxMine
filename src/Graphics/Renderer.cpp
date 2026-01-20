#include "Renderer.h"
#include "../WorldGeneration/WorldManager.hpp"

#include "../Core/Camera.h"
#include "Shader.h"

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

void Renderer::Init() {
    shader = new Shader(
        "../../shaders/vertex.vs",
        "../../shaders/fragment.fs"
    );

float cubeVertices[] = {
    0,0,1,  0,0,1,  0,0,
    1,0,1,  0,0,1,  1,0,
    1,1,1,  0,0,1,  1,1,

    0,0,1,  0,0,1,  0,0,
    1,1,1,  0,0,1,  1,1,
    0,1,1,  0,0,1,  0,1,


    1,0,0,  0,0,-1,  0,0,
    0,0,0,  0,0,-1,  1,0,
    0,1,0,  0,0,-1,  1,1,

    1,0,0,  0,0,-1,  0,0,
    0,1,0,  0,0,-1,  1,1,
    1,1,0,  0,0,-1,  0,1,


    0,0,0,  -1,0,0,  0,0,
    0,0,1,  -1,0,0,  1,0,
    0,1,1,  -1,0,0,  1,1,

    0,0,0,  -1,0,0,  0,0,
    0,1,1,  -1,0,0,  1,1,
    0,1,0,  -1,0,0,  0,1,


    1,0,1,  1,0,0,  0,0,
    1,0,0,  1,0,0,  1,0,
    1,1,0,  1,0,0,  1,1,

    1,0,1,  1,0,0,  0,0,
    1,1,0,  1,0,0,  1,1,
    1,1,1,  1,0,0,  0,1,


    0,1,1,  0,1,0,  0,0,
    1,1,1,  0,1,0,  1,0,
    1,1,0,  0,1,0,  1,1,

    0,1,1,  0,1,0,  0,0,
    1,1,0,  0,1,0,  1,1,
    0,1,0,  0,1,0,  0,1,


    0,0,0,  0,-1,0,  0,0,
    1,0,0,  0,-1,0,  1,0,
    1,0,1,  0,-1,0,  1,1,

    0,0,0,  0,-1,0,  0,0,
    1,0,1,  0,-1,0,  1,1,
    0,0,1,  0,-1,0,  0,1
};


    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);

    glVertexAttribPointer(
        0, 3, GL_FLOAT, GL_FALSE,
        8 * sizeof(float),
        (void*)0
    );
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(
        1, 3, GL_FLOAT, GL_FALSE,
        8 * sizeof(float),
        (void*)(3 * sizeof(float))
    );
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(
        2, 2, GL_FLOAT, GL_FALSE,
        8 * sizeof(float),
        (void*)(6 * sizeof(float))
    );
    glEnableVertexAttribArray(2);


    EnableCullFace();
}

void Renderer::RenderWorld(World& world, const Camera& camera) {
    shader->use();

    glm::mat4 projection = glm::perspective(
        glm::radians(60.f),
        1280.f / 720.f,
        0.1f,
        100.f
    );

    shader->setMat4("projection", projection);
    shader->setMat4("view", camera.GetViewMatrix());

    shader->setVec3("lightDir", -0.3f, -1.0f, -0.2f);
    shader->setVec3("lightColor", 0.9f, 0.9f, 0.9f);
    shader->setVec3("ambientColor", 0.25f, 0.25f, 0.25f);

    glBindVertexArray(vao);

    for (int x = 0; x < world.getSizeX(); x++) {
        for (int y = 0; y < world.getSizeY(); y++) {
            for (int z = 0; z < world.getSizeZ(); z++) {

                BlockType t = world.getBlock(x, y, z);
                if (t == AIR)
                    continue;

                glm::mat4 model =
                    glm::translate(glm::mat4(1.f), glm::vec3(x, y, z));
                shader->setMat4("model", model);

                int layerY = 0;

                switch (t)
                {
                    case DIRT:     layerY = y;    break;
                    case STONE:    layerY = -1;   break;
                    case TREASURE: layerY = 999;  break;
                    default:       continue;
                }

                shader->setInt("layerY", layerY);
                glDrawArrays(GL_TRIANGLES, 0, 36);
            }
        }
    }
}

void Renderer::RenderFallingBlocks(const std::vector<FallingBlock>& blocks,
                                  const Camera& camera)
{
    shader->use();

    glm::mat4 projection = glm::perspective(
        glm::radians(60.f),
        1280.f / 720.f,
        0.1f,
        100.f
    );

    shader->setMat4("projection", projection);
    shader->setMat4("view", camera.GetViewMatrix());

    shader->setVec3("lightDir", -0.3f, -1.0f, -0.2f);
    shader->setVec3("lightColor", 0.9f, 0.9f, 0.9f);
    shader->setVec3("ambientColor", 0.25f, 0.25f, 0.25f);

    glBindVertexArray(vao);

    for (const auto& b : blocks)
    {
        glm::mat4 model = glm::translate(
            glm::mat4(1.f),
            glm::vec3((float)b.x, b.y, (float)b.z)
        );

        shader->setMat4("model", model);
        shader->setInt("layerY", -1);
        glDrawArrays(GL_TRIANGLES, 0, 36);
    }
}

void Renderer::EnableCullFace()
{
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);
}

void Renderer::ToggleWireframe(bool eWireframe)
{
    glPolygonMode(GL_FRONT_AND_BACK, eWireframe ? GL_LINE : GL_FILL);
}
