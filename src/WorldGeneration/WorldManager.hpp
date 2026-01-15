#include "World.hpp"
#include "RandomIntGen.hpp"

class WorldManager
{
    World world;
    RandomIntGen gen;

    int playerX, playerY, playerZ;
    int treasureX, treasureY, treasureZ;

    void updateFallingBlocks(int _x, int _y, int _z);

    void generateWorld(int sizeX, int sizeY, int sizeZ);

    // Checks if the player can stand at the given coordinates.
    bool canPlayerStandAt(int x, int y, int z); 

    // Randomly selects a block type based on probabilities.
    BlockType drawRandomBlock(int x, int y, int z);

    void generatePlayerStartLocation();

    public:
    
    WorldManager(int sizeX, int sizeY, int sizeZ);
    ~WorldManager();

    int getPlayerX() { return playerX; }
    int getPlayerY() { return playerY; }
    int getPlayerZ() { return playerZ; }

    int getTreasureX() { return treasureX; }
    int getTreasureY() { return treasureY; }
    int getTreasureZ() { return treasureZ; }

    // Tries to destroy block at given coordinates. Returns -2 when out of bounds, -1 when cannot destroy block, otherwise blockType.
    int destroyBlockAt(int x, int y, int z); 

    // Checks if route to treasure exists.
    int checkRouteToTreasure();

    // Returns reference to the managed world.
    World& getWorld() {return world;} 
};