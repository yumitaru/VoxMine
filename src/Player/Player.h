#pragma once
class World;
class Camera;

class Player {
public:
    void Update(float dt, World& world, Camera& camera);

private:
    float velocityY = 0.f;
    bool onGround = false;
};
