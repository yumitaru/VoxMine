#pragma once

#include <vector>

struct FallingBlock;
class World;
class Camera;
class Shader;



class Renderer {
public:
    void Init();
    void RenderWorld(World& world, const Camera& camera);
    void EnableCullFace();
    void ToggleWireframe(bool eWireframe);
    void RenderFallingBlocks(const std::vector<FallingBlock>& blocks, const Camera& camera);

private:
    Shader* shader = nullptr;
    unsigned int vao = 0;
    unsigned int vbo = 0;

    unsigned int stoneTex = 0;
    unsigned int mossTex  = 0;
};
