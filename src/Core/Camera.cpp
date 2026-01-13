#include "Camera.h"

Camera::Camera(glm::vec3 startPos)
    : Position(startPos),
      Front(0.f, 0.f, -1.f),
      Up(0.f, 1.f, 0.f),
      Yaw(-90.f),
      Pitch(0.f),
      Speed(5.f),
      Sensitivity(0.1f) {}

glm::mat4 Camera::GetViewMatrix() const {
    return glm::lookAt(Position, Position + Front, Up);
}

void Camera::ProcessKeyboard(int dir, float dt) {
    float v = Speed * dt;
    glm::vec3 right = glm::normalize(glm::cross(Front, Up));

    if (dir == 0) Position += Front * v;
    if (dir == 1) Position -= Front * v;
    if (dir == 2) Position -= right * v;
    if (dir == 3) Position += right * v;
}

void Camera::ProcessMouse(float xoffset, float yoffset) {
    xoffset *= Sensitivity;
    yoffset *= Sensitivity;

    Yaw += xoffset;
    Pitch += yoffset;

    if (Pitch > 89.f) Pitch = 89.f;
    if (Pitch < -89.f) Pitch = -89.f;

    glm::vec3 f;
    f.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    f.y = sin(glm::radians(Pitch));
    f.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    Front = glm::normalize(f);
}
