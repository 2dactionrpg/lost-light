#include <entt/entity/registry.hpp>
#include "../components/motionComponent.hpp"
#include "../components/enemyComponent.hpp"
#include "../components/characterComponent.hpp"
#include "../common.hpp"

struct EnemyAISystem
{
    void set_direction(entt::registry &registry);
    void set_target(entt::registry &registry);
    void set_rotation(entt::registry &registry);

};