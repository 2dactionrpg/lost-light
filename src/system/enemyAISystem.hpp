#pragma once

#include "../common.hpp"
#include "../components/characterComponent.hpp"
#include "../components/enemyComponent.hpp"
#include "../components/motionComponent.hpp"
#include "../data.hpp"
#include "../enemy.hpp"
#include "../zombie.hpp"
#include "../factory.hpp"
#include "../projectile.hpp"
#include "../redzone.hpp"

using namespace std;

struct EnemyAISystem
{
    int proj_counter = 0;
    void update(entt::registry &registry, float elapsed_ms, vector<Enemy> &m_enemies, vector<Zombie> &m_zombies);
    void set_direction(entt::registry &registry);
    void set_target(entt::registry &registry);
    void set_rotation(entt::registry &registry);
    void shoot_manager(entt::registry &registry, float elapsed_ms, vector<Enemy> &m_enemies, vector<Zombie> &m_zombies,     vector<Projectile> &m_projectiles);
    void shoot(entt::registry &registry, int enemy_type, vec2 proj_direction, vec2 face_pos, float randians,                vector<Projectile> &m_projectiles);
    void destroy_dead_enemies(entt::registry &registry);
    void skill_manager(entt::registry &registry, float elapsed_ms, Redzone &m_redzone);
    void summon_redzone(entt::registry &registry, Redzone &redzone);
};
