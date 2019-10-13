#include <entt/entity/registry.hpp>
#include "../components/motionComponent.hpp"
#include "../components/enemyComponent.hpp"
#include "../common.hpp"

struct EnemyAISystem
{
    void set_direction(entt::registry &registry);
};