#include "World.hpp"

class WorldManager
{
    World world;

    void updateFallingBlocks(int _x, int _y, int _z);

    void generateWorld(int sizeX, int sizeY, int sizeZ);

    public:
    
    WorldManager(int sizeX, int sizeY, int sizeZ);
    ~WorldManager();

    // Tries to destroy block at given coordinates. Returns -2 when out of bounds, -1 when cannot destroy block, otherwise blockType.
    int destroyBlockAt(int x, int y, int z); 

    // Checks if route to treasure exists.
    int checkRouteToTreasure();

    // Returns reference to the managed world.
    World& getWorld() {return world;} 
};