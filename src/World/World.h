#pragma once
#include "Block.h"

class World {
public:
    static const int SIZE_X = 16;
    static const int SIZE_Y = 4;
    static const int SIZE_Z = 16;

    BlockType blocks[SIZE_X][SIZE_Y][SIZE_Z];

    World();

    bool isInside(int x, int y, int z) const;
    BlockType get(int x, int y, int z) const;
    void set(int x, int y, int z, BlockType type);
};
