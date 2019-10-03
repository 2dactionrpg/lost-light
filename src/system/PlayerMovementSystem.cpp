#include "PlayerMovementSystem.hpp"

void PlayerMovementSystem::update(entt::registry &registry)
{
    auto view = registry.view<player, motion>();
    for (auto entity : view)
    {
        auto &pos = view.get<motion>(entity).position;
        auto &speed = view.get<motion>(entity).speed;
    }
}