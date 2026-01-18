#include "Block.hpp"
#include <glm/glm.hpp>

class World
{
    int* blocks;

    int size_x, size_y, size_z;

    public:
    
    World(){ World(2 ,2 ,2); }
    World(int sizeX, int sizeY, int sizeZ);
    ~World();

    int getSizeX() { return size_x; }
    int getSizeY() { return size_y; }
    int getSizeZ() { return size_z; }

    BlockType getBlock(int x, int y, int z) const;
    BlockType destroyBlock(int x, int y, int z);
    int setBlock(int x, int y, int z, BlockType block);
    bool isInside(int x,int y,int z);

    void clear();

    int* getBlocks() {return blocks;}
};