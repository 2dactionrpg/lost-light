#pragma once

#include <entt/entity/registry.hpp>
#include "../components/motion.hpp"
#include "../components/player.hpp"

class PlayerMovementSystem
{
private:
public:
    void update(entt::registry &registry);
    PlayerMovementSystem(/* args */);
    ~PlayerMovementSystem();
};

PlayerMovementSystem::PlayerMovementSystem(/* args */)
{
}

PlayerMovementSystem::~PlayerMovementSystem()
{
}
