#pragma once
#include <glm/glm.hpp>

enum class BlockType {
    Air = 0,
    Solid
};

class World {
public:
    static constexpr int SIZE_X = 64;
    static constexpr int SIZE_Y = 8;
    static constexpr int SIZE_Z = 64;

    World();

    bool isInside(int x,int y,int z) const;
    BlockType get(int x,int y,int z) const;
    void set(int x,int y,int z, BlockType t);

    bool Raycast(glm::vec3 o, glm::vec3 d, glm::ivec3& hit) const;

private:
    BlockType blocks[SIZE_X][SIZE_Y][SIZE_Z];
};
