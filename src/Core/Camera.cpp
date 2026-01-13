#include "Camera.h"

Camera::Camera(glm::vec3 startPos)
    : Position(startPos),
      Front(0.0f, 0.0f, -1.0f),
      Up(0.0f, 1.0f, 0.0f),
      Yaw(-90.0f),
      Pitch(0.0f),
      Speed(5.0f),
      Sensitivity(0.1f) {}

glm::mat4 Camera::GetViewMatrix() {
    return glm::lookAt(Position, Position + Front, Up);
}

void Camera::ProcessKeyboard(int dir, float dt) {
    float velocity = Speed * dt;
    glm::vec3 right = glm::normalize(glm::cross(Front, Up));

    if (dir == 0) Position += Front * velocity;   // W
    if (dir == 1) Position -= Front * velocity;   // S
    if (dir == 2) Position -= right * velocity;   // A
    if (dir == 3) Position += right * velocity;   // D
}

void Camera::ProcessMouse(float xoffset, float yoffset) {
    xoffset *= Sensitivity;
    yoffset *= Sensitivity;

    Yaw   += xoffset;
    Pitch += yoffset;

    if (Pitch > 89.0f)  Pitch = 89.0f;
    if (Pitch < -89.0f) Pitch = -89.0f;

    glm::vec3 front;
    front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    front.y = sin(glm::radians(Pitch));
    front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    Front = glm::normalize(front);
}
