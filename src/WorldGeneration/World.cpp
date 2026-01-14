#include "World.hpp"

World::World(int sizeX, int sizeY, int sizeZ)
{
    if(sizeX < 2 || sizeY < 2 || sizeZ < 2)
    {
        blocks = nullptr;
        return;
    }

    size_x = sizeX;
    size_y = sizeY;
    size_z = sizeZ;

    blocks = new int[sizeX * sizeY * sizeZ];

}

World::~World()
{
    if (blocks == nullptr) return;

    delete[] blocks;
}

BlockType World::getBlock(int x, int y, int z) const
{
    return (BlockType)blocks[x*(size_y*size_z) + y*(size_z) + z];
}

BlockType World::destroyBlock(int x, int y, int z)
{
    int destroyed_block = blocks[x*(size_y*size_z) + y*(size_z) + z];
    blocks[x*(size_y*size_z) + y*(size_z) + z] = AIR;
    return (BlockType)destroyed_block;
}

int World::setBlock(int x, int y, int z, BlockType block)
{
    blocks[x*(size_y*size_z) + y*(size_z) + z] = block;
    return block;
}