struct Block
{
    bool solid;
    bool breakable;

    Block(bool _solid, bool _breakable) : solid(_solid), breakable(_breakable) {}
};

enum BlockType
{
    AIR = 0,
    DIRT,
    STONE,
    TREASURE
};