#include "RandomIntGen.hpp"

RandomIntGen::RandomIntGen()
    : mt(rd()), dist({20, 50, 30})
{
}

int RandomIntGen::get()
{
    return dist(mt);
}
