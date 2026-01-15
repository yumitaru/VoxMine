#include <random>

class RandomIntGen
{
    std::random_device rd;
    std::mt19937 mt;
    std::discrete_distribution<int> dist;

public:
    RandomIntGen();
    int get();
};
