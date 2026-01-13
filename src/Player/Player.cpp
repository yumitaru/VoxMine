#include "Player.h"
#include "../World/World.h"
#include "../Core/Camera.h"
#include "../Input/Input.h"
#include <glm/glm.hpp>
#include <cmath>

static constexpr float HEIGHT = 1.f;
static constexpr float GRAVITY = -20.f;
static constexpr float STEP_HEIGHT = 1.f;

void Player::Update(float dt, World& world, Camera& camera) {
    glm::vec3 forward = glm::normalize(glm::vec3(camera.Front.x,0,camera.Front.z));
    glm::vec3 right = glm::normalize(glm::cross(forward, camera.Up));

    float v = camera.Speed * dt;
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

    if (world.isInside(tx, ty, tz) && world.get(tx, ty, tz) == BlockType::Solid) {
        int stepY = ty + 1;
        if (world.isInside(tx, stepY, tz) &&
            world.get(tx, stepY, tz) == BlockType::Air) {
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

    if (world.isInside(tx, ty, tz) && world.get(tx, ty, tz) == BlockType::Solid) {
        int stepY = ty + 1;
        if (world.isInside(tx, stepY, tz) &&
            world.get(tx, stepY, tz) == BlockType::Air) {
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

    if (world.isInside(x,y,z) && world.get(x,y,z)==BlockType::Solid) {
        camera.Position.y = y + HEIGHT + 1.f;
        velocityY = 0;
        onGround = true;
    }

    if (Input::Key(GLFW_KEY_SPACE) && onGround) {
        velocityY = 8.f;
        onGround = false;
    }

    if (camera.Position.x < 0.1f) camera.Position.x = 0.1f;
    if (camera.Position.x > World::SIZE_X - 0.1f)
        camera.Position.x = World::SIZE_X - 0.1f;

    if (camera.Position.z < 0.1f) camera.Position.z = 0.1f;
    if (camera.Position.z > World::SIZE_Z - 0.1f)
        camera.Position.z = World::SIZE_Z - 0.1f;

    if (camera.Position.y < HEIGHT) {
        camera.Position.y = HEIGHT;
        velocityY = 0;
        onGround = true;
    }
}
