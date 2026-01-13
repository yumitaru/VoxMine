#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Core/Camera.h"
#include "Graphics/Shader.h"
#include "World/World.h"

World world;

int playerLayer = 4;

Camera camera({8.f, playerLayer + 1.f, 8.f});

float lastX = 400.f, lastY = 300.f;
bool firstMouse = true;
float deltaTime = 0.f, lastFrame = 0.f;

unsigned int cubeVAO, cubeVBO;


bool raycast(World& w, glm::vec3 origin, glm::vec3 dir, glm::ivec3& hit) {
    for (float t = 0.f; t < 6.f; t += 0.1f) {
        glm::vec3 p = origin + dir * t;

        int x = (int)floor(p.x);
        int y = (int)floor(p.y);
        int z = (int)floor(p.z);

        if (w.isInside(x, y, z) && w.get(x, y, z) == BlockType::Solid) {
            hit = { x, y, z };
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
        if (!raycast(world, camera.Position, camera.Front, hit))
            return;

        world.set(hit.x, hit.y, hit.z, BlockType::Air);

        int px = (int)floor(camera.Position.x);
        int pz = (int)floor(camera.Position.z);

        if (hit.x == px && hit.z == pz && hit.y == playerLayer) {
            if (playerLayer > 0)
                playerLayer--;
        }
    }
}


void processInput(GLFWwindow* w) {

    if (glfwGetKey(w, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(w, true);
        return;
    }

    glm::vec3 oldPos = camera.Position;

    if (glfwGetKey(w, GLFW_KEY_W) == GLFW_PRESS)
        camera.Position += camera.Front * deltaTime * camera.Speed;
    if (glfwGetKey(w, GLFW_KEY_S) == GLFW_PRESS)
        camera.Position -= camera.Front * deltaTime * camera.Speed;

    glm::vec3 right = glm::normalize(glm::cross(camera.Front, camera.Up));
    if (glfwGetKey(w, GLFW_KEY_A) == GLFW_PRESS)
        camera.Position -= right * deltaTime * camera.Speed;
    if (glfwGetKey(w, GLFW_KEY_D) == GLFW_PRESS)
        camera.Position += right * deltaTime * camera.Speed;

    int x = (int)floor(camera.Position.x);
    int z = (int)floor(camera.Position.z);

    if (!world.isInside(x, 0, z)) {
        camera.Position = oldPos;
    }

    camera.Position.y = (float)playerLayer + 1.f;
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

    float cubeVertices[] = {
        0,0,1, 1,0,1, 1,1,1,  0,0,1, 1,1,1, 0,1,1,
        1,0,0, 0,0,0, 0,1,0,  1,0,0, 0,1,0, 1,1,0,
        0,0,0, 0,0,1, 0,1,1,  0,0,0, 0,1,1, 0,1,0,
        1,0,1, 1,0,0, 1,1,0,  1,0,1, 1,1,0, 1,1,1,
        0,1,1, 1,1,1, 1,1,0,  0,1,1, 1,1,0, 0,1,0,
        0,0,0, 1,0,0, 1,0,1,  0,0,0, 1,0,1, 0,0,1
    };

    glGenVertexArrays(1, &cubeVAO);
    glGenBuffers(1, &cubeVBO);

    glBindVertexArray(cubeVAO);
    glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    while (!glfwWindowShouldClose(window)) {
        float t = (float)glfwGetTime();
        deltaTime = t - lastFrame;
        lastFrame = t;

        processInput(window);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4 projection = glm::perspective(
            glm::radians(60.f),
            800.f / 600.f,
            0.1f,
            100.f
        );

        glm::mat4 view = camera.GetViewMatrix();

        shader.use();
        shader.setMat4("projection", projection);
        shader.setMat4("view", view);

        for (int x = 0; x < World::SIZE_X; x++)
            for (int y = 0; y < World::SIZE_Y; y++)
                for (int z = 0; z < World::SIZE_Z; z++)
                    if (world.get(x, y, z) == BlockType::Solid) {
                        glm::mat4 model = glm::translate(glm::mat4(1.f), glm::vec3(x, y, z));
                        shader.setMat4("model", model);
                        glBindVertexArray(cubeVAO);
                        glDrawArrays(GL_TRIANGLES, 0, 36);
                    }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
