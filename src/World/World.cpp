#include "World.h"
#include <cmath>
#include <cstring>
#include <cstdlib>


static inline float Hash(int x, int z) {
    int n = x * 73856093 ^ z * 19349663;
    n = (n << 13) ^ n;
    return 1.f - ((n * (n * n * 15731 + 789221) + 1376312589)
        & 0x7fffffff) / 1073741824.f;
}


World::World() {
    std::memset(blocks, 0, sizeof(blocks));

    for (int x = 0; x < SIZE_X; x++) {
        for (int z = 0; z < SIZE_Z; z++) {

            float n = Hash(x / 4, z / 4);
            int height = (int)(n * 2 + SIZE_Y / 2);

            if (height < 1) height = 1;
            if (height >= SIZE_Y) height = SIZE_Y - 1;

            for (int y = 0; y <= height; y++) {
                blocks[x][y][z] = BlockType::Solid;
            }

            if (height + 1 < SIZE_Y && (rand() % 6 == 0)) {
                blocks[x][height + 1][z] = BlockType::Falling;
            }
        }
    }
}

bool World::isInside(int x,int y,int z) const {
    return x>=0 && y>=0 && z>=0 &&
           x<SIZE_X && y<SIZE_Y && z<SIZE_Z;
}

BlockType World::get(int x,int y,int z) const {
    return blocks[x][y][z];
}

void World::set(int x,int y,int z, BlockType t) {
    if (isInside(x,y,z))
        blocks[x][y][z] = t;
}

void World::UpdateFallingBlocks(float) {
    for (int x = 0; x < SIZE_X; x++) {
        for (int z = 0; z < SIZE_Z; z++) {
            for (int y = 1; y < SIZE_Y; y++) {

                if (blocks[x][y][z] == BlockType::Falling &&
                    blocks[x][y - 1][z] == BlockType::Air) {

                    blocks[x][y - 1][z] = BlockType::Falling;
                    blocks[x][y][z] = BlockType::Air;
                }
            }
        }
    }
}


bool World::Raycast(glm::vec3 o, glm::vec3 d, glm::ivec3& hit) const {
    glm::ivec3 p = glm::floor(o);

    glm::vec3 step = glm::sign(d);
    glm::vec3 tDelta = glm::abs(1.f / d);
    glm::vec3 next;

    next.x = ((step.x > 0 ? (p.x + 1) - o.x : o.x - p.x) * tDelta.x);
    next.y = ((step.y > 0 ? (p.y + 1) - o.y : o.y - p.y) * tDelta.y);
    next.z = ((step.z > 0 ? (p.z + 1) - o.z : o.z - p.z) * tDelta.z);

    for (int i = 0; i < 32; i++) {
        if (isInside(p.x, p.y, p.z) && get(p.x,p.y,p.z) != BlockType::Air) {
            hit = p;
            return true;
        }

        if (next.x < next.y && next.x < next.z) {
            p.x += (int)step.x;
            next.x += tDelta.x;
        } else if (next.y < next.z) {
            p.y += (int)step.y;
            next.y += tDelta.y;
        } else {
            p.z += (int)step.z;
            next.z += tDelta.z;
        }
    }
    return false;
}
