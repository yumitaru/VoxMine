struct FallingBlock
{
    int x, z;
    float y;        // aktualna wysokość (float = płynny ruch)
    int targetY;    // gdzie ma spaść
    float velocity; // prędkość spadania
};