#include "collisionSystem.hpp"

void CollisionSystem::update(entt::registry& registry, Character& m_character, Shield& m_shield, vector<Enemy>& m_enemies, vector<Projectile>& m_projectiles, Potion& m_potion, unsigned int& m_points)
{
    // Checking Character - Potion collisions
    if (m_character.collides_with(m_potion) && !m_potion.is_consumed()) {
        physicsSystem.consume_potion(registry, m_potion.get_id());
        physicsSystem.set_shield_scale_multiplier(registry, 2.0f, 1.0f);
    }

    auto projectile_it = m_projectiles.begin();
    while (projectile_it != m_projectiles.end()) {

        // check boundry conditions
        float w = projectile_it->get_bounding_box().x / 2;
        float h = projectile_it->get_bounding_box().y / 2;
        if (projectile_it->get_position().x + w < 0.f || projectile_it->get_position().x - w > 1200.f || projectile_it->get_position().y + h < 0.f || projectile_it->get_position().y - h > 850.f) {
            projectile_it = m_projectiles.erase(projectile_it);
            continue;
        }

        // check shield collision
        if (m_shield.collides_with(*projectile_it)) {
            vec2 shieldDirection = m_shield.get_direction();
            vec2 projectileDirection = projectile_it->get_direction();
            vec2 reflection = sub(
                projectileDirection,
                mul(mul(shieldDirection, 2.f), dot(shieldDirection, shieldDirection)));
            physicsSystem.reflect_projectile(registry, projectile_it->get_id(), reflection);
            ++projectile_it;
            continue;
        }

        // check character collision
        if (m_character.collides_with(*projectile_it)) {
            soundSystem.play_sound(C_DEAD);
            physicsSystem.set_character_unmovable(registry);
            projectile_it = m_projectiles.erase(projectile_it);
            menuSystem.sync(registry, STATE_GAMEOVER);
            continue;
        }

        // check enemy collision
        bool hits_enemy = false;
        auto enemy_it = m_enemies.begin();
        while (enemy_it != m_enemies.end()) {
            if (enemy_it->collides_with(*projectile_it)) {
                m_points++;
                hits_enemy = true;
                break;
            } else {
                ++enemy_it;
            }
        }

        if (hits_enemy) {
            healthSystem.damage(registry, enemy_it->get_id());
            projectile_it = m_projectiles.erase(projectile_it);
        } else {
            ++projectile_it;
        }
    }
}
