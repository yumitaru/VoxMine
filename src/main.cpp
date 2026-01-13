#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Core/Camera.h"
#include "Graphics/Shader.h"
#include "World/World.h"

// ================= KONFIG =================

const float PLAYER_HEIGHT = 1.0f;
const float GRAVITY = -20.f;

// ================= GLOBALS =================

World world;
Camera camera({8.f, 20.f, 8.f});

float velocityY = 0.f;
bool onGround = false;

float lastX = 400.f, lastY = 300.f;
bool firstMouse = true;
float deltaTime = 0.f, lastFrame = 0.f;

unsigned int cubeVAO, cubeVBO;

// --- HUD ---
unsigned int hudVAO, hudVBO;
Shader* hudShader;

// ================= RAYCAST =================

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

// ================= CALLBACKI =================

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

// ================= FIZYKA Y =================

void applyPhysicsY(float dt) {
    int x = (int)floor(camera.Position.x);
    int z = (int)floor(camera.Position.z);

    float feetY = camera.Position.y - PLAYER_HEIGHT;
    int yBelow = (int)floor(feetY);

    if (world.isInside(x, yBelow, z) &&
        world.get(x, yBelow, z) == BlockType::Solid) {

        camera.Position.y = (float)yBelow + 2.0f;
        velocityY = 0.f;
        onGround = true;
        return;
    }

    onGround = false;
    velocityY += GRAVITY * dt;
    camera.Position.y += velocityY * dt;

    if (camera.Position.y < 2.0f) {
        camera.Position.y = 2.0f;
        velocityY = 0.f;
        onGround = true;
    }
}

// ================= KOLIZJA BOCZNA + STEP-UP =================

bool canMoveTo(glm::vec3 newPos) {
    int x = (int)floor(newPos.x);
    int z = (int)floor(newPos.z);

    float feetY = camera.Position.y - PLAYER_HEIGHT;
    int y = (int)floor(feetY);

    if (!world.isInside(x, y, z))
        return false;

    if (world.get(x, y, z) == BlockType::Solid) {
        if (world.isInside(x, y + 1, z) &&
            world.get(x, y + 1, z) == BlockType::Air) {

            camera.Position.y += 1.0f;
            return true;
        }
        return false;
    }
    return true;
}

// ================= INPUT =================

void processInput(GLFWwindow* w) {
    if (glfwGetKey(w, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(w, true);
        return;
    }

    float v = camera.Speed * deltaTime;
    glm::vec3 forward = glm::normalize(glm::vec3(camera.Front.x, 0.f, camera.Front.z));
    glm::vec3 right = glm::normalize(glm::cross(forward, camera.Up));

    glm::vec3 newPos;

    if (glfwGetKey(w, GLFW_KEY_W) == GLFW_PRESS) {
        newPos = camera.Position + forward * v;
        if (canMoveTo(newPos)) camera.Position = newPos;
    }
    if (glfwGetKey(w, GLFW_KEY_S) == GLFW_PRESS) {
        newPos = camera.Position - forward * v;
        if (canMoveTo(newPos)) camera.Position = newPos;
    }
    if (glfwGetKey(w, GLFW_KEY_A) == GLFW_PRESS) {
        newPos = camera.Position - right * v;
        if (canMoveTo(newPos)) camera.Position = newPos;
    }
    if (glfwGetKey(w, GLFW_KEY_D) == GLFW_PRESS) {
        newPos = camera.Position + right * v;
        if (canMoveTo(newPos)) camera.Position = newPos;
    }

    if (glfwGetKey(w, GLFW_KEY_SPACE) == GLFW_PRESS && onGround) {
        int x = (int)floor(camera.Position.x);
        int z = (int)floor(camera.Position.z);
        float feetY = camera.Position.y - PLAYER_HEIGHT;
        int y = (int)floor(feetY);

        if (world.isInside(x, y + 1, z) &&
            world.get(x, y + 1, z) == BlockType::Air) {

            camera.Position.y += 1.0f;
        }
    }

    applyPhysicsY(deltaTime);
}

// ================= MAIN =================

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
    hudShader = new Shader("../../shaders/hud.vs", "../../shaders/hud.fs");

    // ===== HUD VAO =====
    float hudVerts[] = {
        // obwódka
        -0.02f,  0.02f,
         0.02f,  0.02f,
         0.02f, -0.02f,
        -0.02f, -0.02f,
        // środek
        -0.01f,  0.01f,
         0.01f,  0.01f,
         0.01f, -0.01f,
        -0.01f, -0.01f
    };

    glGenVertexArrays(1, &hudVAO);
    glGenBuffers(1, &hudVBO);
    glBindVertexArray(hudVAO);
    glBindBuffer(GL_ARRAY_BUFFER, hudVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(hudVerts), hudVerts, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // ===== KOSTKA =====
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

        glm::mat4 projection = glm::perspective(glm::radians(60.f), 800.f/600.f, 0.1f, 100.f);
        glm::mat4 view = camera.GetViewMatrix();

        shader.use();
        shader.setMat4("projection", projection);
        shader.setMat4("view", view);

        for (int x = 0; x < World::SIZE_X; x++)
            for (int y = 0; y < World::SIZE_Y; y++)
                for (int z = 0; z < World::SIZE_Z; z++)
                    if (world.get(x, y, z) == BlockType::Solid) {
                        glm::mat4 model = glm::translate(glm::mat4(1.f), glm::vec3(x,y,z));
                        shader.setMat4("model", model);
                        glUniform1i(glGetUniformLocation(shader.ID,"layerY"), y);
                        glBindVertexArray(cubeVAO);
                        glDrawArrays(GL_TRIANGLES, 0, 36);
                    }

        // ===== CELOWNIK =====
        glDisable(GL_DEPTH_TEST);
        hudShader->use();
        glBindVertexArray(hudVAO);

        glUniform3f(glGetUniformLocation(hudShader->ID,"color"),0,0,0);
        glDrawArrays(GL_TRIANGLE_FAN,0,4);

        glUniform3f(glGetUniformLocation(hudShader->ID,"color"),1,1,1);
        glDrawArrays(GL_TRIANGLE_FAN,4,4);

        glEnable(GL_DEPTH_TEST);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
