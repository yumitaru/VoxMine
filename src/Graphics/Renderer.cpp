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
        0,0,1, 1,0,1, 1,1,1,  0,0,1, 1,1,1, 0,1,1,
        1,0,0, 0,0,0, 0,1,0,  1,0,0, 0,1,0, 1,1,0,
        0,0,0, 0,0,1, 0,1,1,  0,0,0, 0,1,1, 0,1,0,
        1,0,1, 1,0,0, 1,1,0,  1,0,1, 1,1,0, 1,1,1,
        0,1,1, 1,1,1, 1,1,0,  0,1,1, 1,1,0, 0,1,0,
        0,0,0, 1,0,0, 1,0,1,  0,0,0, 1,0,1, 0,0,1
    };

    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
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

    glBindVertexArray(vao);

    for (int x = 0; x < world.getSizeX(); x++) {
        for (int y = 0; y < world.getSizeY(); y++) {
            for (int z = 0; z < world.getSizeZ(); z++) {

                BlockType t = world.getBlock(x, y, z);
                if (t == BlockType::AIR)
                    continue;

                glm::mat4 model =
                    glm::translate(glm::mat4(1.f), glm::vec3(x, y, z));

                shader->setMat4("model", model);

                int layer = (t == BlockType::STONE) ? -1 : y;
                shader->setInt("layerY", layer);

                glDrawArrays(GL_TRIANGLES, 0, 36);
            }
        }
    }
}
