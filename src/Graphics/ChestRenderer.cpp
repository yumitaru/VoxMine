#include "ChestRenderer.h"
#include "Shader.h"
#include "Model.h"

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

void ChestRenderer::Init()
{
    std::cout << "ChestRenderer Init\n";

    shader = new Shader(
        "../../shaders/model.vs",
        "../../shaders/model.fs"
    );

    model = new Model("../../src/Models/treasure-chest.obj");
}

void ChestRenderer::Render(const glm::vec3& position,
                           const glm::mat4& view,
                           const glm::mat4& proj)
{

    if (!shader || !model)
        return;
    shader->use();

    glm::mat4 m(1.f);
    m = glm::translate(m, position);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, model->GetTextureID());
    shader->setInt("diffuseTex", 0);


    shader->setMat4("model", m);
    shader->setMat4("view", view);
    shader->setMat4("projection", proj);

    shader->setVec3("lightDir", -0.3f, -1.0f, -0.2f);
    shader->setVec3("lightColor", 0.9f, 0.9f, 0.9f);
    shader->setVec3("ambientColor", 0.6f, 0.6f, 0.6f);

    model->Draw();
}
