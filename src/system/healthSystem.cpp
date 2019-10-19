#include "healthSystem.hpp"
#include <iostream>

void HealthSystem::update(entt::registry& registry, vector<Enemy>& m_enemies, int& enemiesKilled)
{
    // Enemy Physics Update
    auto enemy = registry.view<enemyComponent>();

    for (auto entity : enemy) {
        auto& id = enemy.get(entity).id;
        auto& health = enemy.get(entity).health;
        if (health < 1) {
            auto enemy_it = m_enemies.begin();
            while (enemy_it != m_enemies.end()) {
                if (enemy_it->get_id() == id) {
                    enemiesKilled++;
                    m_enemies.erase(enemy_it);
                    break;
                }
                ++enemy_it;
            }
        }
    }
}

void HealthSystem::damage(entt::registry& registry, int m_id)
{
    // Enemy Physics Update
    auto enemy = registry.view<enemyComponent>();

    for (auto entity : enemy) {
        auto& id = enemy.get(entity).id;
        auto& health = enemy.get(entity).health;
        if (id == m_id) {
            health--;
        }
    }
}