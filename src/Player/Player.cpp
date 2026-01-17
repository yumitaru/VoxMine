#include "Player.h"
#include "../WorldGeneration/World.hpp"
#include "../Core/Camera.h"
#include "../Input/Input.h"
#include <glm/glm.hpp>
#include <cmath>

static constexpr float HEIGHT = 1.f;
static constexpr float GRAVITY = -20.f;
static constexpr float STEP_HEIGHT = 1.f;

void Player::Update(float dt, World& world, Camera& camera) {

    float scroll = Input::ScrollDY();
    if (scroll != 0.f) {
        toolHidden = !toolHidden;
    }

    glm::vec3 forward = glm::normalize(glm::vec3(camera.Front.x,0,camera.Front.z));
    glm::vec3 right = glm::normalize(glm::cross(forward, camera.Up));

    float speed = toolHidden ? camera.Speed * 1.35f : camera.Speed;
    float v = speed * dt;

    glm::vec3 np = camera.Position;

    if (Input::Key(GLFW_KEY_W)) np += forward * v;
    if (Input::Key(GLFW_KEY_S)) np -= forward * v;
    if (Input::Key(GLFW_KEY_A)) np -= right * v;
    if (Input::Key(GLFW_KEY_D)) np += right * v;

    glm::vec3 test = camera.Position;
    test.x = np.x;

    int tx = (int)floor(test.x);
    int ty = (int)floor(camera.Position.y - HEIGHT);
    int tz = (int)floor(test.z);

    BlockType b = world.getBlock(tx, ty, tz);

    if (world.isInside(tx, ty, tz) && (b == BlockType::STONE || b == BlockType::DIRT || b == BlockType::TREASURE)) {
        int stepY = ty + 1;
        if (world.isInside(tx, stepY, tz) &&
            world.getBlock(tx, stepY, tz) == BlockType::AIR) {
            camera.Position.y += STEP_HEIGHT;
        }
    } else {
        camera.Position.x = test.x;
    }

    test = camera.Position;
    test.z = np.z;

    tx = (int)floor(test.x);
    ty = (int)floor(camera.Position.y - HEIGHT);
    tz = (int)floor(test.z);

    b = world.getBlock(tx, ty, tz);

    if (world.isInside(tx, ty, tz) && (b == BlockType::STONE || b == BlockType::DIRT || b == BlockType::TREASURE)) {
        int stepY = ty + 1;
        if (world.isInside(tx, stepY, tz) &&
            world.getBlock(tx, stepY, tz) == BlockType::AIR) {
            camera.Position.y += STEP_HEIGHT;
        }
    } else {
        camera.Position.z = test.z;
    }

    velocityY += GRAVITY * dt;
    camera.Position.y += velocityY * dt;

    int x = (int)floor(camera.Position.x);
    int y = (int)floor(camera.Position.y - HEIGHT);
    int z = (int)floor(camera.Position.z);

    b = world.getBlock(x, y, z);

    if (world.isInside(x,y,z) && (b == BlockType::STONE || b == BlockType::DIRT || b == BlockType::TREASURE)) {
        camera.Position.y = y + HEIGHT + 1.f;
        velocityY = 0;
        onGround = true;
    }

    if (Input::Key(GLFW_KEY_SPACE) && onGround) {
        velocityY = 8.f;
        onGround = false;
    }

    if (camera.Position.x < 0.1f) camera.Position.x = 0.1f;
    if (camera.Position.x > world.getSizeX() - 0.1f)
        camera.Position.x = world.getSizeX() - 0.1f;

    if (camera.Position.z < 0.1f) camera.Position.z = 0.1f;
    if (camera.Position.z > world.getSizeZ() - 0.1f)
        camera.Position.z = world.getSizeZ() - 0.1f;

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
        if (world.isInside(p.x, p.y, p.z) && world.getBlock(p.x,p.y,p.z) != BlockType::AIR) {
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
