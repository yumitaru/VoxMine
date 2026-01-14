#include "WorldManager.hpp"

void WorldManager::updateFallingBlocks(int _x, int _y, int _z)
{
    for(int y = 1; y < world.getSizeY(); y++)
    {
        if(world.getBlock(_x, y, _z) == STONE)
        {
            int check_y = y - 1;
            while(check_y >= 0)
            {
                if(world.getBlock(_x, check_y, _z) == AIR)
                {
                    check_y--;
                }
                else break;
            }
            if(check_y != y - 1)
            {
                world.setBlock(_x, check_y + 1, _z, STONE);
                world.setBlock(_x, y, _z, AIR);
            }
        }
    }
}

int WorldManager::checkRouteToTreasure()
{
    // TODO: implement A*
    return 0;
}

void WorldManager::generateWorld(int sizeX, int sizeY, int sizeZ)
{
    // TODO: generate based on algorithm 
    for(int x = 0; x < sizeX; x++)
    {
        for(int y = 0; y < sizeY; y++)
        {
            for(int z = 0; z < sizeZ; z++)
            {
                if(y == 0 || y == 4 || y == 2 || y == 6)
                {
                    world.setBlock(x, y, z, STONE);
                }
                else if(y == 5 || y == 3 || y == 1)
                {
                    world.setBlock(x, y, z, AIR);
                }
                else
                {
                    world.setBlock(x, y, z, DIRT);
                }
            }
        }
    }
}

WorldManager::WorldManager(int sizeX, int sizeY, int sizeZ)
    : world(sizeX, sizeY, sizeZ)
{
    generateWorld(sizeX, sizeY, sizeZ);
}


WorldManager::~WorldManager()
{
}

int WorldManager::destroyBlockAt(int x, int y, int z)
{
    if (x >= world.getSizeX() || y >= world.getSizeY() || z >= world.getSizeZ() || x < 0 || y < 0 || z < 0) return -2;
        
    if (world.getBlock(x, y, z) == DIRT) 
    {
        int destroyed_block = world.destroyBlock(x, y, z);
        updateFallingBlocks(x, y, z);
        return destroyed_block;
    }

    return -1;
}
