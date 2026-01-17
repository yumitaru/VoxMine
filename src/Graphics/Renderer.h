#pragma once


class World;
class Camera;
class Shader;

class Renderer {
public:
    void Init();
    void RenderWorld(World& world, const Camera& camera);
    void EnableCullFace();
    void ToggleWireframe(bool eWireframe);

private:
    Shader* shader = nullptr;
    unsigned int vao = 0;
    unsigned int vbo = 0;
};
