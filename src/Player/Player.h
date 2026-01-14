#pragma once
class World;
class Camera;

class Player {
public:
    void Update(float dt, World& world, Camera& camera);

    bool IsToolHidden() const { return toolHidden; }

private:
    float velocityY = 0.f;
    bool onGround = false;
    bool toolHidden = false;
};
