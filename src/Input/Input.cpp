#include "Input.h"

GLFWwindow* Input::window = nullptr;
double Input::lastX = 0;
double Input::lastY = 0;
float Input::dx = 0;
float Input::dy = 0;
bool Input::firstMouse = true;

float Input::scrollDY = 0.f;

void Input::Init(GLFWwindow* w) {
    window = w;
    glfwSetScrollCallback(window, ScrollCallback);
}

void Input::Update() {
    double x, y;
    glfwGetCursorPos(window, &x, &y);

    if (firstMouse) {
        lastX = x;
        lastY = y;
        firstMouse = false;
    }

    dx = (float)(x - lastX);
    dy = (float)(lastY - y);

    lastX = x;
    lastY = y;
}

bool Input::Key(int key) {
    return glfwGetKey(window, key) == GLFW_PRESS;
}

bool Input::MousePressed(int button) {
    return glfwGetMouseButton(window, button) == GLFW_PRESS;
}

float Input::MouseDX() { return dx; }
float Input::MouseDY() { return dy; }

float Input::ScrollDY() {
    float v = scrollDY;
    scrollDY = 0.f;
    return v;
}

void Input::ScrollCallback(GLFWwindow*, double, double yoffset) {
    scrollDY = (float)yoffset;
}
