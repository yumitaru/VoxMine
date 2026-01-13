#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Core/Camera.h"
#include "Graphics/Shader.h"
#include "World/World.h"

Camera camera({8.f, 2.f, 20.f});
World world;

float lastX = 400, lastY = 300;
bool firstMouse = true;
float deltaTime = 0.f, lastFrame = 0.f;

unsigned int cubeVAO, cubeVBO;


bool raycast(World& w, glm::vec3 o, glm::vec3 d, glm::ivec3& hit) {
    for (float t = 0; t < 20.f; t += 0.1f) {
        glm::vec3 p = o + d * t;
        int x = (int)floor(p.x);
        int y = (int)floor(p.y);
        int z = (int)floor(p.z);
        if (w.isInside(x, y, z) && w.get(x, y, z) == BlockType::Solid) {
            hit = {x, y, z};
            return true;
        }
    }
    return false;
}

void mouse_callback(GLFWwindow*, double x, double y) {
    if (firstMouse) {
        lastX = (float)x;
        lastY = (float)y;
        firstMouse = false;
    }
    camera.ProcessMouse((float)x - lastX, lastY - (float)y);
    lastX = (float)x;
    lastY = (float)y;
}

void mouse_button_callback(GLFWwindow*, int button, int action, int) {
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        glm::ivec3 hit;
        if (raycast(world, camera.Position, camera.Front, hit)) {
            world.set(hit.x, hit.y, hit.z, BlockType::Air);
        }
    }
}


void processInput(GLFWwindow* w) {

    if (glfwGetKey(w, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(w, true);
        return;
    }

    if (glfwGetKey(w, GLFW_KEY_W) == GLFW_PRESS) camera.ProcessKeyboard(0, deltaTime);
    if (glfwGetKey(w, GLFW_KEY_S) == GLFW_PRESS) camera.ProcessKeyboard(1, deltaTime);
    if (glfwGetKey(w, GLFW_KEY_A) == GLFW_PRESS) camera.ProcessKeyboard(2, deltaTime);
    if (glfwGetKey(w, GLFW_KEY_D) == GLFW_PRESS) camera.ProcessKeyboard(3, deltaTime);

    camera.Position.y = 2.f;
}


int main() {
    glfwInit();

    GLFWwindow* window = glfwCreateWindow(800, 600, "VoxelMine", nullptr, nullptr);
    glfwMakeContextCurrent(window);

    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    glEnable(GL_DEPTH_TEST);

    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    Shader shader("../../shaders/vertex.vs", "../../shaders/fragment.fs");

    float cube[] = {
        0,0,0,  1,0,0,  1,1,0,  0,1,0,
        0,0,1,  1,0,1,  1,1,1,  0,1,1
    };

    glGenVertexArrays(1, &cubeVAO);
    glGenBuffers(1, &cubeVBO);
    glBindVertexArray(cubeVAO);
    glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cube), cube, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    while (!glfwWindowShouldClose(window)) {
        float t = (float)glfwGetTime();
        deltaTime = t - lastFrame;
        lastFrame = t;

        processInput(window);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4 proj = glm::perspective(glm::radians(60.f), 800.f / 600.f, 0.1f, 100.f);
        glm::mat4 view = camera.GetViewMatrix();

        shader.use();
        shader.setMat4("projection", proj);
        shader.setMat4("view", view);

        for (int x = 0; x < World::SIZE_X; x++)
            for (int y = 0; y < World::SIZE_Y; y++)
                for (int z = 0; z < World::SIZE_Z; z++)
                    if (world.get(x, y, z) == BlockType::Solid) {
                        glm::mat4 m = glm::translate(glm::mat4(1.f), {x, y, z});
                        shader.setMat4("model", m);
                        glBindVertexArray(cubeVAO);
                        glDrawArrays(GL_LINE_LOOP, 0, 8);
                    }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
