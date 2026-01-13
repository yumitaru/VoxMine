#pragma once
#include <GLFW/glfw3.h>

class Input {
public:
    static void Init(GLFWwindow* w);
    static void Update();

    static bool Key(int key);
    static bool MousePressed(int button);

    static float MouseDX();
    static float MouseDY();

private:
    static GLFWwindow* window;
    static double lastX, lastY;
    static float dx, dy;
    static bool firstMouse;
};
