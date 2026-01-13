#include "Application.h"

#include <iostream>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include "../Input/Input.h"
#include "../World/World.h"
#include "../Player/Player.h"
#include "../Core/Camera.h"
#include "../Graphics/Renderer.h"
#include "../Graphics/HUDRenderer.h"

static World world;
static Player player;
static Camera camera({8.f, 20.f, 8.f});
static Renderer renderer;
static HUDRenderer hud;

static float lastFrame = 0.f;
static bool mouseLeftWasDown = false;

Application::Application() {}
Application::~Application() { Shutdown(); }

void Application::Init() {
    glfwSetErrorCallback([](int e, const char* d) {
        std::cerr << "GLFW ERROR " << e << ": " << d << "\n";
    });

    if (!glfwInit()) {
        running = false;
        return;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(1280, 720, "VoxelMine", nullptr, nullptr);
    if (!window) {
        running = false;
        return;
    }

    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        running = false;
        return;
    }

    glEnable(GL_DEPTH_TEST);
    glClearColor(0.6f, 0.7f, 0.9f, 1.f);

    Input::Init(window);
    renderer.Init();
    hud.Init();

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void Application::Run() {
    Init();
    if (!running) return;

    while (!glfwWindowShouldClose(window)) {
        float t = (float)glfwGetTime();
        float dt = t - lastFrame;
        lastFrame = t;

        glfwPollEvents();
        Input::Update();

        // ================= ESC = ZAMKNIJ GRĘ =================
        if (Input::Key(GLFW_KEY_ESCAPE)) {
            glfwSetWindowShouldClose(window, true);
        }
        // =====================================================

        camera.ProcessMouse(Input::MouseDX(), Input::MouseDY());
        player.Update(dt, world, camera);

        // 1 klik = 1 blok
        bool mouseDown = Input::MousePressed(GLFW_MOUSE_BUTTON_LEFT);
        if (mouseDown && !mouseLeftWasDown) {
            glm::ivec3 hit;
            if (world.Raycast(camera.Position, camera.Front, hit)) {
                world.set(hit.x, hit.y, hit.z, BlockType::Air);
            }
        }
        mouseLeftWasDown = mouseDown;

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        renderer.RenderWorld(world, camera);
        hud.Render();

        glfwSwapBuffers(window);
    }
}

void Application::Shutdown() {
    if (window) glfwDestroyWindow(window);
    glfwTerminate();
}
