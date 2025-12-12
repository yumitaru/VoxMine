#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

int main()
{
    glfwInit();

    GLFWwindow* window = glfwCreateWindow(800, 600, "Test", nullptr, nullptr);
    glfwMakeContextCurrent(window);

    glm::vec3 v(1.0f, 2.0f, 3.0f);

    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
