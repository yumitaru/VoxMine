#include "Floor.h"

Floor::Floor(float s) : size(s) {}

bool Floor::IsInside(float x, float z) {
    return x >= -size && x <= size && z >= -size && z <= size;
}
