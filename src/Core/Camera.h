#pragma once
#include <glm/glm.hpp>

class Camera {
public:
    Camera(glm::vec3 pos);

    glm::mat4 GetViewMatrix() const;
    void ProcessMouse(float dx, float dy);

    glm::vec3 Position;
    glm::vec3 Front{0,0,-1};
    glm::vec3 Up{0,1,0};

    float Yaw = -90.f;
    float Pitch = 0.f;
    float Speed = 6.f;
};
