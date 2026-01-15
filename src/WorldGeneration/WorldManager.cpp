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
    // random x and z for treasure
    int _treasureX = gen.get() * (sizeX - 1) / 2;
    int _treasureZ = gen.get() * (sizeZ - 1) / 2;

    for(int y = 0; y < sizeY; y++)
    {
        for(int z = 0; z < sizeZ; z++)
        {
            for(int x = 0; x < sizeX; x++)
            {
                BlockType block = drawRandomBlock(x, y, z);
                world.setBlock(x, y, z, block);

                if(x == _treasureX && z == _treasureZ)
                {
                    if(y == 0)
                    {
                        world.setBlock(x, y, z, TREASURE);
                        treasureX = x;
                        treasureY = y;
                        treasureZ = z;
                        continue;
                    }
                    world.setBlock(x, y, z, DIRT);
                    continue;
                }
            }
        }
    }
}

bool WorldManager::canPlayerStandAt(int x, int y, int z)
{
    if(world.getBlock(x, y, z) != AIR) return false;

    if( y + 1 < world.getSizeY() && world.getBlock(x, y + 1, z) != AIR) return false;

    if(y - 1 > 0 && world.getBlock(x, y - 1, z) == AIR) return false;

    return true;
}

BlockType WorldManager::drawRandomBlock(int x, int y, int z)
{
    if(y == 0) return STONE;

    if (world.getBlock(x, y - 1, z) == AIR) return DIRT;

    return (BlockType)gen.get();
}

void WorldManager::generatePlayerStartLocation()
{
    for(int x = 0; x < world.getSizeX(); x++)
    {
        for(int z = 0; z < world.getSizeZ(); z++)
        {
            for(int y = world.getSizeY() - 1; y >= 0; y--)
            {
                if(canPlayerStandAt(x, y, z))
                {
                    playerX = x;
                    playerY = y;
                    playerZ = z;
                    return;
                }
            }
        }
    }
}

WorldManager::WorldManager(int sizeX, int sizeY, int sizeZ)
    : world(sizeX, sizeY, sizeZ)
{
    generateWorld(sizeX, sizeY, sizeZ);
    generatePlayerStartLocation();
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
