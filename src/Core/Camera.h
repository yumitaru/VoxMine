#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera {
public:
    glm::vec3 Position;
    glm::vec3 Front;
    glm::vec3 Up;

    float Yaw;
    float Pitch;
    float Speed;
    float Sensitivity;

    Camera(glm::vec3 startPos);

    glm::mat4 GetViewMatrix() const;
    void ProcessKeyboard(int dir, float dt);
    void ProcessMouse(float xoffset, float yoffset);
};
