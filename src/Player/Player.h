#pragma once

#include <glm/glm.hpp>
class World;
class Camera;

class Player {
public:
    void Update(float dt, World& world, Camera& camera);

    bool IsToolHidden() const { return toolHidden; }

    bool raycast(glm::vec3 o, glm::vec3 d, glm::ivec3& hit, World& world);

private:
    float velocityY = 0.f;
    bool onGround = false;
    bool toolHidden = false;
};
