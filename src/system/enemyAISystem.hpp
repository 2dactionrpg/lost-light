#include <entt/entity/registry.hpp>
#include "../components/motionComponent.hpp"
#include "../components/enemyComponent.hpp"
#include "../components/characterComponent.hpp"
#include "../common.hpp"
#include "../enemy.hpp"
#include "../projectile.hpp"

using namespace std;

struct EnemyAISystem
{
    void set_direction(entt::registry &registry);
    void set_target(entt::registry &registry);
    void set_rotation(entt::registry &registry);
    void shoot(entt::registry &registry, float elapsed_ms, vector<Enemy> &m_enemies, vector<Projectile> &m_projectiles);
};