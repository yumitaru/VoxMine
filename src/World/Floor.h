#pragma once

class Floor {
public:
    float size;
    Floor(float size);
    bool IsInside(float x, float z);
};
