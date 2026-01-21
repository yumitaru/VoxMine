#pragma once

struct GLFWwindow;

class Application {
public:
    Application();
    ~Application();

    void Run();


private:
    void Init();
    void Shutdown();
    void ResetGame();

    GLFWwindow* window = nullptr;
    bool running = true;
    bool gameWon = false;
};
