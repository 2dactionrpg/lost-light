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
            fprintf(stderr, "%f %f\n", destination.x, destination.y);
            direction = normalize(sub(destination, position));
            fprintf(stderr, "%f %f\n", direction.x, direction.y);
        }
        else if (position.x == destination.x  && position.y == destination.y)
        {
            direction = {0.f, 0.f};
        }
        // direction = add(direction, {0.f, 1.f});
        // direction = add(direction, {1.f, 0.f});
        // direction = {1.f, 1.f};
    }
}