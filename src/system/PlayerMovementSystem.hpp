#pragma once

#include <entt/entity/registry.hpp>
#include "../components/motion.hpp"
#include "../components/input.hpp"
#include "../components/player.hpp"

class PlayerMovementSystem
{
private:
public:
    void update(entt::registry &registry, float elapsed_ms);
    void move(vec2 pos, vec2 off);
    void set_rotation(float radians, float newRadians);
    PlayerMovementSystem(/* args */);
    ~PlayerMovementSystem();
};

PlayerMovementSystem::PlayerMovementSystem(/* args */)
{
}

PlayerMovementSystem::~PlayerMovementSystem()
{
}
