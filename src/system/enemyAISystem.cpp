#include "enemyAISystem.hpp"

float LO = 100.f;
float HI_X = 1100.f;
float HI_Y = 700.f;

void EnemyAISystem::set_direction(entt::registry &registry)
{
    auto view = registry.view<enemyComponent, motionComponent>();
    for (auto entity : view)
    {
        auto &[position, direction, radians, speed] = view.get<motionComponent>(entity);
        auto &destination = view.get<enemyComponent>(entity).destination;
        if (direction.x == 0.f && direction.y == 0.f)
        {
            // set new destination
            // update direction
            destination.x = LO + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (HI_X - LO)));
            destination.y = LO + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (HI_Y - LO)));
            direction = normalize(sub(destination, position));
        }
        else if (position.x == destination.x  && position.y == destination.y)
        {
            direction = {0.f, 0.f};
        }
    }
}

void EnemyAISystem::set_target(entt::registry &registry)
{
    vec2 char_pos;
    auto character_view = registry.view<characterComponent, motionComponent>();
    for (auto character : character_view)
    {
        char_pos = character_view.get<motionComponent>(character).position;
    }

    auto enemy_view = registry.view<enemyComponent, motionComponent>();
    for (auto entity : enemy_view)
    {
        auto &target = enemy_view.get<enemyComponent>(entity).target;
        target = char_pos;
    }
}

void EnemyAISystem::set_rotation(entt::registry &registry)
{
    auto view = registry.view<enemyComponent, motionComponent>();
    for (auto entity : view)
    {
        auto &[position, direction, radians, speed] = view.get<motionComponent>(entity);
        auto &target = view.get<enemyComponent>(entity).target;
        radians = atan2(target.x - position.x, position.y - target.y);
    }
}