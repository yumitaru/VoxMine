#pragma once
#include <GLFW/glfw3.h>

class Renderer;

class Input {
public:
    static void Init(GLFWwindow* w);
    static void Update();

    static bool Key(int key);
    static bool MousePressed(int button);

    static float MouseDX();
    static float MouseDY();

    static float ScrollDY();
    static void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset);
    static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

private:
    static GLFWwindow* window;
    static double lastX, lastY;
    static float dx, dy;
    static bool firstMouse;
    static bool eWireframe;

    static float scrollDY;
};
