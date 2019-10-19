#include "../common.hpp"
#include "../components/characterComponent.hpp"
#include "../components/enemyComponent.hpp"
#include "../components/motionComponent.hpp"
#include "../enemy.hpp"
#include "../factory.hpp"
#include "../projectile.hpp"

using namespace std;

struct EnemyAISystem {
    int proj_counter = 0;
    void set_direction(entt::registry& registry);
    void set_target(entt::registry& registry);
    void set_rotation(entt::registry& registry);
    void shoot(entt::registry& registry, float elapsed_ms, vector<Enemy>& m_enemies, vector<Projectile>& m_projectiles);
    void destroy_dead_enemies(entt::registry& registry);
};
