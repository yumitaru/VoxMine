#pragma once
#include <glm/glm.hpp>

class Shader;
class Model;

class ChestRenderer {
public:
    void Init();
    void Render(const glm::vec3& position,
                const glm::mat4& view,
                const glm::mat4& proj);

private:
    Shader* shader = nullptr;
    Model* model = nullptr;
};
