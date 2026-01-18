#include "Player.h"
#include "../WorldGeneration/World.hpp"
#include "../Core/Camera.h"
#include "../Input/Input.h"
#include <glm/glm.hpp>
#include <cmath>

static constexpr float HEIGHT = 1.f;
static constexpr float GRAVITY = -20.f;
static constexpr float JUMP_FORCE = 8.f;

static inline bool isSolid(BlockType b)
{
    return b == BlockType::STONE ||
           b == BlockType::DIRT ||
           b == BlockType::TREASURE;
}

static inline bool isPassableForPlayer(BlockType b)
{
    return b == BlockType::AIR;
}

void Player::Update(float dt, World& world, Camera& camera)
{
    {
        int x = (int)floor(camera.Position.x);
        int y = (int)floor(camera.Position.y - HEIGHT);
        int z = (int)floor(camera.Position.z);

        int safety = 0;
        while (world.isInside(x,y,z) && isSolid(world.getBlock(x,y,z)) && safety < 8) {
            camera.Position.y += 1.f;
            y = (int)floor(camera.Position.y - HEIGHT);
            safety++;
        }
    }

    bool steppedUp = false;

    float scroll = Input::ScrollDY();
    if (scroll != 0.f)
        toolHidden = !toolHidden;

    glm::vec3 forward = glm::normalize(glm::vec3(camera.Front.x, 0, camera.Front.z));
    glm::vec3 right   = glm::normalize(glm::cross(forward, camera.Up));

    float speed = toolHidden ? camera.Speed * 1.35f : camera.Speed;
    float v = speed * dt;

    glm::vec3 nextPos = camera.Position;

    if (Input::Key(GLFW_KEY_W)) nextPos += forward * v;
    if (Input::Key(GLFW_KEY_S)) nextPos -= forward * v;
    if (Input::Key(GLFW_KEY_A)) nextPos -= right   * v;
    if (Input::Key(GLFW_KEY_D)) nextPos += right   * v;


    if (Input::Key(GLFW_KEY_SPACE) && onGround)
    {
        velocityY = JUMP_FORCE;
        onGround = false;
    }


    {
        glm::vec3 test = camera.Position;
        test.x = nextPos.x;

        int fx = (int)floor(test.x);
        int fy = (int)floor(camera.Position.y - HEIGHT);
        int hy = fy + 1;
        int z  = (int)floor(test.z);

        bool footBlocked =
            world.isInside(fx,fy,z) &&
            !isPassableForPlayer(world.getBlock(fx,fy,z));

        bool headBlocked =
            world.isInside(fx,hy,z) &&
            !isPassableForPlayer(world.getBlock(fx,hy,z));

        if (footBlocked || headBlocked)
        {
            int stepY = fy + 1;

            if (!headBlocked &&
                (!world.isInside(fx, stepY + 1, z) ||
                 isPassableForPlayer(world.getBlock(fx, stepY + 1, z))))
            {
                camera.Position.y = stepY + HEIGHT;
                camera.Position.x = test.x;
                steppedUp = true;
            }
        }
        else
        {
            camera.Position.x = test.x;
        }
    }

    {
        glm::vec3 test = camera.Position;
        test.z = nextPos.z;

        int fz = (int)floor(test.z);
        int fy = (int)floor(camera.Position.y - HEIGHT);
        int hy = fy + 1;
        int x  = (int)floor(test.x);

        bool footBlocked =
            world.isInside(x,fy,fz) &&
            !isPassableForPlayer(world.getBlock(x,fy,fz));

        bool headBlocked =
            world.isInside(x,hy,fz) &&
            !isPassableForPlayer(world.getBlock(x,hy,fz));

        if (footBlocked || headBlocked)
        {
            int stepY = fy + 1;

            if (!headBlocked &&
                (!world.isInside(x, stepY + 1, fz) ||
                 isPassableForPlayer(world.getBlock(x, stepY + 1, fz))))
            {
                camera.Position.y = stepY + HEIGHT;
                camera.Position.z = test.z;
                steppedUp = true;
            }
        }
        else
        {
            camera.Position.z = test.z;
        }
    }

    velocityY += GRAVITY * dt;
    camera.Position.y += velocityY * dt;

    {
        int x = (int)floor(camera.Position.x);
        int y = (int)floor(camera.Position.y - HEIGHT);
        int z = (int)floor(camera.Position.z);

        if (world.isInside(x,y,z) && isSolid(world.getBlock(x,y,z)))
        {
            camera.Position.y = y + HEIGHT + 1.f;
            velocityY = 0.f;
            onGround = true;
        }
    }

    {
        float minX = 0.f;
        float minZ = 0.f;
        float maxX = (float)world.getSizeX() - 0.001f;
        float maxZ = (float)world.getSizeZ() - 0.001f;

        if (camera.Position.x < minX) camera.Position.x = minX;
        if (camera.Position.z < minZ) camera.Position.z = minZ;
        if (camera.Position.x > maxX) camera.Position.x = maxX;
        if (camera.Position.z > maxZ) camera.Position.z = maxZ;
    }

    if (camera.Position.y < HEIGHT)
    {
        camera.Position.y = HEIGHT;
        velocityY = 0.f;
        onGround = true;
    }
}

bool Player::raycast(glm::vec3 o, glm::vec3 d, glm::ivec3 &hit, World& world)
{
    glm::ivec3 p = glm::floor(o);

    glm::vec3 step = glm::sign(d);
    glm::vec3 tDelta = glm::abs(1.f / d);
    glm::vec3 next;

    next.x = ((step.x > 0 ? (p.x + 1) - o.x : o.x - p.x) * tDelta.x);
    next.y = ((step.y > 0 ? (p.y + 1) - o.y : o.y - p.y) * tDelta.y);
    next.z = ((step.z > 0 ? (p.z + 1) - o.z : o.z - p.z) * tDelta.z);

    for (int i = 0; i < 32; i++)
    {
        if (world.isInside(p.x,p.y,p.z) &&
            world.getBlock(p.x,p.y,p.z) != BlockType::AIR)
        {
            hit = p;
            return true;
        }

        if (next.x < next.y && next.x < next.z) {
            p.x += (int)step.x;
            next.x += tDelta.x;
        }
        else if (next.y < next.z) {
            p.y += (int)step.y;
            next.y += tDelta.y;
        }
        else {
            p.z += (int)step.z;
            next.z += tDelta.z;
        }
    }
    return false;
}
