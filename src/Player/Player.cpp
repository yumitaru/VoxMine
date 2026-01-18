#include "Player.h"
#include "../WorldGeneration/World.hpp"
#include "../Core/Camera.h"
#include "../Input/Input.h"
#include <glm/glm.hpp>
#include <cmath>

static constexpr float HEIGHT = 1.f;
static constexpr float GRAVITY = -20.f;

// BLOKI BLOKUJĄCE
static inline bool isSolid(BlockType b) {
    return b == BlockType::STONE ||
           b == BlockType::DIRT ||
           b == BlockType::TREASURE;
}

static inline bool isPassableForPlayer(BlockType b)
{
    return b == BlockType::AIR;
}

void Player::Update(float dt, World& world, Camera& camera) {

    // =====================================================
    // ANTI-STUCK (RESPAWN W BLOKU)
    // =====================================================
    {
        int x = (int)floor(camera.Position.x);
        int y = (int)floor(camera.Position.y - HEIGHT);
        int z = (int)floor(camera.Position.z);

        int safety = 0;
        while (world.isInside(x,y,z) &&
               isSolid(world.getBlock(x,y,z)) &&
               safety < 8)
        {
            camera.Position.y += 1.f;
            y = (int)floor(camera.Position.y - HEIGHT);
            safety++;
        }
    }

    bool steppedUp = false;

    float scroll = Input::ScrollDY();
    if (scroll != 0.f)
        toolHidden = !toolHidden;

    glm::vec3 forward = glm::normalize(glm::vec3(camera.Front.x,0,camera.Front.z));
    glm::vec3 right   = glm::normalize(glm::cross(forward, camera.Up));

    float speed = toolHidden ? camera.Speed * 1.35f : camera.Speed;
    float v = speed * dt;

    glm::vec3 np = camera.Position;

    if (Input::Key(GLFW_KEY_W)) np += forward * v;
    if (Input::Key(GLFW_KEY_S)) np -= forward * v;
    if (Input::Key(GLFW_KEY_A)) np -= right   * v;
    if (Input::Key(GLFW_KEY_D)) np += right   * v;

    // =====================================================
    // OŚ X + KOLIZJA GŁOWY
    // =====================================================
    {
        glm::vec3 test = camera.Position;
        test.x = np.x;

        int footX = (int)floor(test.x);
        int footY = (int)floor(camera.Position.y - HEIGHT);
        int headY = footY + 1;
        int z     = (int)floor(test.z);

        bool footBlocked =
            world.isInside(footX,footY,z) &&
            !isPassableForPlayer(world.getBlock(footX,footY,z));

        bool headBlocked =
            world.isInside(footX,headY,z) &&
            !isPassableForPlayer(world.getBlock(footX,headY,z));

        if (footBlocked || headBlocked) {
            int stepY = footY + 1;

            if (!headBlocked &&
                (!world.isInside(footX, stepY + 1, z) ||
                 world.getBlock(footX, stepY + 1, z) == BlockType::AIR))
            {
                camera.Position.y = stepY + HEIGHT;
                camera.Position.x = test.x;
                steppedUp = true;
            }
        } else {
            camera.Position.x = test.x;
        }
    }

    // =====================================================
    // OŚ Z + KOLIZJA GŁOWY
    // =====================================================
    {
        glm::vec3 test = camera.Position;
        test.z = np.z;

        int footZ = (int)floor(test.z);
        int footY = (int)floor(camera.Position.y - HEIGHT);
        int headY = footY + 1;
        int x     = (int)floor(test.x);

        bool footBlocked =
            world.isInside(x,footY,footZ) &&
            !isPassableForPlayer(world.getBlock(x,footY,footZ));

        bool headBlocked =
            world.isInside(x,headY,footZ) &&
            !isPassableForPlayer(world.getBlock(x,headY,footZ));

        if (footBlocked || headBlocked) {
            int stepY = footY + 1;

            if (!headBlocked &&
                (!world.isInside(x, stepY + 1, footZ) ||
                 world.getBlock(x, stepY + 1, footZ) == BlockType::AIR))
            {
                camera.Position.y = stepY + HEIGHT;
                camera.Position.z = test.z;
                steppedUp = true;
            }
        } else {
            camera.Position.z = test.z;
        }
    }

    // =====================================================
    // GRAWITACJA
    // =====================================================
    if (!steppedUp) {
        velocityY += GRAVITY * dt;
        camera.Position.y += velocityY * dt;
    } else {
        velocityY = 0.f;
        onGround = true;
    }

    // =====================================================
    // PODŁOŻE
    // =====================================================
    {
        int x = (int)floor(camera.Position.x);
        int y = (int)floor(camera.Position.y - HEIGHT);
        int z = (int)floor(camera.Position.z);

        if (world.isInside(x,y,z) && isSolid(world.getBlock(x,y,z))) {
            camera.Position.y = y + HEIGHT + 1.f;
            velocityY = 0;
            onGround = true;
        }
    }

    if (camera.Position.y < HEIGHT) {
        camera.Position.y = HEIGHT;
        velocityY = 0;
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

    for (int i = 0; i < 32; i++) {
        if (world.isInside(p.x,p.y,p.z) &&
            world.getBlock(p.x,p.y,p.z) != BlockType::AIR) {
            hit = p;
            return true;
        }

        if (next.x < next.y && next.x < next.z) {
            p.x += (int)step.x;
            next.x += tDelta.x;
        } else if (next.y < next.z) {
            p.y += (int)step.y;
            next.y += tDelta.y;
        } else {
            p.z += (int)step.z;
            next.z += tDelta.z;
        }
    }
    return false;
}
