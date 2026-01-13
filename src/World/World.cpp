#include "World.h"

World::World() {
    for (int x = 0; x < SIZE_X; x++)
        for (int y = 0; y < SIZE_Y; y++)
            for (int z = 0; z < SIZE_Z; z++)
                blocks[x][y][z] = (y == 0) ? BlockType::Solid : BlockType::Air;
}

bool World::isInside(int x, int y, int z) const {
    return x >= 0 && y >= 0 && z >= 0 &&
           x < SIZE_X && y < SIZE_Y && z < SIZE_Z;
}

BlockType World::get(int x, int y, int z) const {
    return blocks[x][y][z];
}

void World::set(int x, int y, int z, BlockType type) {
    if (isInside(x, y, z))
        blocks[x][y][z] = type;
}
