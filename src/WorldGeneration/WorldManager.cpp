#include "WorldManager.hpp"
#include <queue>
#include <vector>
#include <functional>


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

bool isPassable(BlockType b)
{
    return b == AIR || b == DIRT || b == TREASURE;
}

bool isSolid(BlockType b)
{
    return b == STONE || b == DIRT || b == TREASURE;
}

int baseCost(BlockType b)
{
    if(b == AIR) return 1;
    if(b == TREASURE) return 1;
    if(b == DIRT) return 6;
    return 1000000; // stone
}

bool WorldManager::hasStoneAbove(int x,int y,int z)
{
    if(y + 1 >= world.getSizeY()) return false;
    return world.getBlock(x, y + 1, z) == STONE;
}


int WorldManager::checkRouteToTreasure()
{
    struct Node {
        int x,y,z;
        int g,f;
    };

    int sx = playerX;
    int sy = playerY;
    int sz = playerZ;

    int tx = treasureX;
    int tz = treasureZ;
    int ty = treasureY + 1;

    int W = world.getSizeX();
    int H = world.getSizeY();
    int D = world.getSizeZ();

    std::priority_queue<Node, std::vector<Node>, 
        std::function<bool(Node,Node)>> open(
        [](const Node&a,const Node&b){return a.f>b.f;}
    );

    static bool closed[64][64][64];   // adjust to max world size
    memset(closed,false,sizeof(closed));

    open.push({sx,sy,sz,0,0});

    while(!open.empty())
    {
        Node n = open.top();
        open.pop();

        if(n.x==tx && n.y==ty && n.z==tz)
            return 1;   // reachable

        if(closed[n.x][n.y][n.z]) continue;
        closed[n.x][n.y][n.z] = true;

        static int dx[6]={1,-1,0,0,0,0};
        static int dy[6]={0,0,0,0,1,-1};
        static int dz[6]={0,0,1,-1,0,0};

        for(int i=0;i<6;i++)
        {
            int nx=n.x+dx[i];
            int ny=n.y+dy[i];
            int nz=n.z+dz[i];

            if(nx < 0 || nz < 0 || nx >= W || nz >= D)
                continue;

            if(ny < 1 || ny >= H)
                continue;


            if(!canPlayerStandAt(nx,ny,nz)) continue;
            if(closed[nx][ny][nz]) continue;

            BlockType b = world.getBlock(nx,ny,nz);
            int cost = baseCost(b);

            if(b==DIRT && hasStoneAbove(nx,ny,nz))
                cost += 50;

            int g = n.g + cost;
            int h = abs(nx-tx)+abs(ny-ty)+abs(nz-tz);
            int f = g + h;

            open.push({nx,ny,nz,g,f});
        }
    }

    return 0;   // unreachable
}

void WorldManager::generateWorld(int sizeX, int sizeY, int sizeZ)
{ 
    // 'random' x and z for treasure
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
    if(x == treasureX && y == treasureY && z == treasureZ) return true;    

    if(!isPassable(world.getBlock(x, y, z))) return false;

    if(y + 1 < world.getSizeY() && !isPassable(world.getBlock(x, y + 1, z))) return false;

    if(y - 1 >= 0 && !isSolid(world.getBlock(x, y - 1, z))) return false;

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

    while(!checkRouteToTreasure())
    {
        world.clear();
        generateWorld(sizeX, sizeY, sizeZ);
        generatePlayerStartLocation();
    }   
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

