#include "WorldManager.hpp"
#include <queue>
#include <vector>
#include <functional>

void WorldManager::updateFallingBlocks(int x, int y, int z)
{
    for(int yy = y + 1; yy < world.getSizeY(); yy++)
    {
        if(world.getBlock(x, yy, z) == STONE)
        {
            int target = yy - 1;
            while (target >= 0 &&
                world.getBlock(x, target, z) == AIR &&
                !isReservedByFallingBlock(x, target, z))
                {
                    target--;
                }

            target++;

            if(target != yy)
            {
                world.setBlock(x, yy, z, AIR);

                fallingBlocks.push_back({
                    x,
                    z,
                    (float)yy,
                    target,
                    0.0f
                });
            }
        }
    }
}

void WorldManager::updateFallingAnimation(float dt)
{
    const float gravity = 20.0f;

    for (auto it = fallingBlocks.begin(); it != fallingBlocks.end(); )
    {
        it->velocity += gravity * dt;
        it->y -= it->velocity * dt;

        if (it->y <= it->targetY)
        {
            world.setBlock(it->x, it->targetY, it->z, STONE);
            it = fallingBlocks.erase(it);
        }
        else
        {
            ++it;
        }
    }
}

bool WorldManager::isReservedByFallingBlock(int x, int y, int z) const
{
    for (const auto& b : fallingBlocks)
    {
        if (b.x == x && b.z == z && b.targetY == y)
            return true;
    }
    return false;
}

const std::vector<FallingBlock> &WorldManager::getFallingBlocks() const
{
    return fallingBlocks;
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
    int _treasureX = std::rand() % world.getSizeX();
    int _treasureZ = std::rand() % world.getSizeZ();

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
    int y = world.getSizeY() - 1;

    while (true)
    {
        int x = std::rand() % world.getSizeX();
        int z = std::rand() % world.getSizeZ();

        if (canPlayerStandAt(x, y, z))
        {
            playerX = x;
            playerY = y;
            playerZ = z;
            return;
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

void WorldManager::Reset(int sizeX, int sizeY, int sizeZ)
{
    world.clear();

    generateWorld(sizeX, sizeY, sizeZ); 
    generatePlayerStartLocation();       

    while (!checkRouteToTreasure())      
    {
        world.clear();
        generateWorld(sizeX, sizeY, sizeZ);
        generatePlayerStartLocation();
    }
}
