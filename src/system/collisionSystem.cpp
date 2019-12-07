#include <components/skillComponent.hpp>
#include "collisionSystem.hpp"

void CollisionSystem::update(entt::registry &registry, Character &m_character, Shield &m_shield, vector<Enemy> &m_enemies, vector<Zombie> &m_zombies, vector<Projectile> &m_projectiles, Potion &m_potion, unsigned int &m_points, vector<Wall> &m_walls, Redzone &m_redzone, float elapsed_ms)
{
    // Checking Character - Potion collisions
    if (m_character.collides_with(m_potion) && !m_potion.is_consumed())
    {
        physicsSystem.consume_potion(registry, m_potion.get_id());
        physicsSystem.set_shield_scale_multiplier(registry, 2.0f, 1.0f);
    }
    // Checking Character - red zone collisions
    auto enemies = registry.view<enemyComponent, motionComponent, skillComponent>();
    for (auto enemy : enemies)
    {
        auto &[cooldown, duration] = enemies.get<skillComponent>(enemy);
        if (m_character.collides_with(m_redzone) && duration < 0.f)
        {
            m_redzone.set_position({-100.f, -100.f});
            soundSystem.play_sound(C_DEAD);
            physicsSystem.set_character_unmovable(registry);
            menuSystem.sync(registry, STATE_GAMEOVER);
        }
    }


    // Checking Zombie - Shield collisions
    auto zombie_it = m_zombies.begin();
    while (zombie_it != m_zombies.end())
    {
        if (m_shield.collides_with(*zombie_it))
        {
            m_points++;
            healthSystem.damage(registry, zombie_it->get_id());
        }
        ++zombie_it;
    }

    // Check Zombie - Character colllisions
    zombie_it = m_zombies.begin();
    while (zombie_it != m_zombies.end())
    {
        if (m_character.collides_with(*zombie_it))
        {
            soundSystem.play_sound(C_HIT);
            m_character.take_damage();
            if (m_character.get_health() < 1)
            {
                soundSystem.play_sound(C_DEAD);
                physicsSystem.set_character_unmovable(registry);
                menuSystem.sync(registry, STATE_GAMEOVER);
            }
            break;
        }
        ++zombie_it;
    }

    auto projectile = registry.view<projectileComponent, motionComponent>();

    for (auto pj1 : projectile)
    {
        auto &id1 = projectile.get<projectileComponent>(pj1).id;
        auto &r1 = projectile.get<projectileComponent>(pj1).radius;
        auto &is_alive = projectile.get<projectileComponent>(pj1).is_alive;

        auto &p1 = projectile.get<motionComponent>(pj1).position;
        auto &d1 = projectile.get<motionComponent>(pj1).direction;

        if (p1.x < -100.f || p1.x > 1300.f || p1.y < -100.f || p1.y > 900.f)
        {
            registry.destroy(pj1);
            continue;
        }

        for (auto pj2 : projectile)
        {
            auto &id2 = projectile.get<projectileComponent>(pj2).id;
            auto &r2 = projectile.get<projectileComponent>(pj2).radius;

            auto &p2 = projectile.get<motionComponent>(pj2).position;
            auto &d2 = projectile.get<motionComponent>(pj2).direction;

            if (id1 == id2)
            {
                continue;
            }

            float x_diff = p1.x - p2.x;
            float y_diff = p1.y - p2.y;

            float d = len({x_diff, y_diff});

            if (d < r1 + r2)
            {
                float adjust_dist = r1 + r2 - d;
                if (p1.y > p2.y)
                {
                    p2.y -= adjust_dist * y_diff / d;
                }
                else
                {
                    p2.y += adjust_dist * x_diff / d;
                }

                if (p1.x < p2.x)
                {
                    p2.x += adjust_dist * x_diff / d;
                }
                else
                {
                    p1.x += adjust_dist * x_diff / d;
                }

                // for collision
                float m1 = r1;
                float m2 = r2;

                vec2 d1_new = add(mul(d1, (m1 - m2) / (m1 + m2)), mul(d2, 2.f * m2 / (m1 + m2)));
                vec2 d2_new = add(mul(d2, (m2 - m1) / (m1 + m2)), mul(d1, 2.f * m1 / (m1 + m2)));

                d1 = d1_new;
                d2 = d2_new;
                break;
            }
        }
    }

    auto projectile_it = m_projectiles.begin();
    while (projectile_it != m_projectiles.end())
    {
        // check boundry conditions
        float w = projectile_it->get_bounding_box().x / 2;
        float h = projectile_it->get_bounding_box().y / 2;
        if (projectile_it->get_position().x + w < 0.f ||
            projectile_it->get_position().x - w > 1200.f ||
            projectile_it->get_position().y + h < 0.f ||
            projectile_it->get_position().y - h > 850.f)
        {
            projectile_it = m_projectiles.erase(projectile_it);
            continue;
        }

        // check wall collision
        bool shouldBreak = false;
        auto wall_it = m_walls.begin();
        for (Wall wall : m_walls)
        {
            if (wall.collides_with(*projectile_it))
            {
                projectile_it = m_projectiles.erase(projectile_it);
                shouldBreak = true;
                break;
            }
        }
        if (shouldBreak)
        {
            break;
        }

        // check shield collision
        if (m_shield.collides_with(*projectile_it))
        {
            vec2 shieldDirection = m_shield.get_direction();
            vec2 projectileDirection = projectile_it->get_direction();
            vec2 reflection = sub(
                projectileDirection,
                mul(mul(shieldDirection, 2.f), dot(shieldDirection, shieldDirection)));
            physicsSystem.reflect_projectile(registry, projectile_it->get_id(), reflection);
            ++projectile_it;
            continue;
        }

        bool hits_character = false;
        // check character collision
        if (m_character.collides_with(*projectile_it))
        {
            soundSystem.play_sound(C_HIT);
            projectile_it = m_projectiles.erase(projectile_it);
            m_character.take_damage();
            hits_character = true;
            if (m_character.get_health() < 1)
            {
                soundSystem.play_sound(C_DEAD);
                physicsSystem.set_character_unmovable(registry);
                menuSystem.sync(registry, STATE_GAMEOVER);
            }
            continue;
        }

        // check enemy collision
        bool hits_enemy = false;
        auto enemy_it = m_enemies.begin();
        while (enemy_it != m_enemies.end())
        {
            if (enemy_it->collides_with(*projectile_it))
            {
                m_points++;
                hits_enemy = true;
                healthSystem.damage(registry, enemy_it->get_id());
                break;
            }
            else
            {
                ++enemy_it;
            }
        }

        bool hits_zombie = false;
        zombie_it = m_zombies.begin();
        while (zombie_it != m_zombies.end())
        {
            if (zombie_it->collides_with(*projectile_it))
            {
                m_points++;
                hits_enemy = true;
                healthSystem.damage(registry, zombie_it->get_id());
                break;
            }
            else
            {
                ++zombie_it;
            }
        }

        if (hits_enemy || hits_zombie || hits_character)
        {
            projectile_it = m_projectiles.erase(projectile_it);
        }
        else
        {
            ++projectile_it;
        }
    }

    for (auto pj : projectile)
    {
        auto &id = projectile.get<projectileComponent>(pj).id;
        auto projectile_it = m_projectiles.begin();
        bool is_found = false;
        while (projectile_it != m_projectiles.end())
        {
            if (projectile_it->get_id() == id) {
                is_found = true;
                break;
            }

            ++projectile_it;
        }
        if (!is_found)
            registry.destroy(pj);
    }
}
