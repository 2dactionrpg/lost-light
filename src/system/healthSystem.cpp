#include "healthSystem.hpp"
#include <iostream>

void HealthSystem::update(entt::registry &registry, vector<Enemy> &m_enemies, vector<Zombie> &m_zombies)
{
    // Enemy Physics Update
    auto enemy = registry.view<enemyComponent>();
    for (auto entity : enemy)
    {
        auto &id = enemy.get(entity).id;
        auto &health = enemy.get(entity).health;
        if (health < 1)
        {
            auto enemy_it = m_enemies.begin();
            while (enemy_it != m_enemies.end())
            {
                if (enemy_it->get_id() == id)
                {
                    levelSystem.increment_enemy_killed(registry);
                    m_enemies.erase(enemy_it);
                    break;
                }
                ++enemy_it;
            }
            auto zombie_it = m_zombies.begin();
            while (zombie_it != m_zombies.end())
            {
                if (zombie_it->get_id() == id)
                {
                    levelSystem.increment_enemy_killed(registry);
                    m_zombies.erase(zombie_it);
                    break;
                }
                ++zombie_it;
            }
        }
    }
}

void HealthSystem::damage(entt::registry &registry, int m_id)
{
    // Enemy Physics Update
    auto enemy = registry.view<enemyComponent>();

    for (auto entity : enemy)
    {
        auto &id = enemy.get(entity).id;
        auto &is_alive = enemy.get(entity).is_alive;
        auto &health = enemy.get(entity).health;
        if (id == m_id)
        {
            health--;
            if (health <= 0)
            {
                is_alive = false;
            }
        }
    }
}
