#include "Camera.h"
#include <glm/gtc/matrix_transform.hpp>

Camera::Camera(glm::vec3 pos) : Position(pos) {}

glm::mat4 Camera::GetViewMatrix() const {
    return glm::lookAt(Position, Position + Front, Up);
}

void Camera::ProcessMouse(float dx, float dy) {
    float sens = 0.1f;
    dx *= sens;
    dy *= sens;

    Yaw += dx;
    Pitch += dy;

    if (Pitch > 89.f) Pitch = 89.f;
    if (Pitch < -89.f) Pitch = -89.f;

    glm::vec3 f;
    f.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    f.y = sin(glm::radians(Pitch));
    f.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    Front = glm::normalize(f);
}
