#include "World.h"
#include <cmath>

World::World() {
    for(int x=0;x<SIZE_X;x++)
        for(int y=0;y<SIZE_Y;y++)
            for(int z=0;z<SIZE_Z;z++)
                blocks[x][y][z] = (y < 4) ? BlockType::Solid : BlockType::Air;
}

bool World::isInside(int x,int y,int z) const {
    return x>=0&&y>=0&&z>=0&&x<SIZE_X&&y<SIZE_Y&&z<SIZE_Z;
}

BlockType World::get(int x,int y,int z) const {
    return blocks[x][y][z];
}

void World::set(int x,int y,int z, BlockType t) {
    if (isInside(x,y,z)) blocks[x][y][z]=t;
}

bool World::Raycast(glm::vec3 o, glm::vec3 d, glm::ivec3& hit) const {
    for(float t=0;t<6;t+=0.1f) {
        glm::vec3 p=o+d*t;
        int x=(int)floor(p.x);
        int y=(int)floor(p.y);
        int z=(int)floor(p.z);
        if(isInside(x,y,z)&&get(x,y,z)==BlockType::Solid){
            hit={x,y,z};
            return true;
        }
    }
    return false;
}
