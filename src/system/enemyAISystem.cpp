#include "enemyAISystem.hpp"

float LO = 100.f;
float HI_X = 1100.f;
float HI_Y = 700.f;

void EnemyAISystem::set_direction(entt::registry& registry)
{
    auto view = registry.view<enemyComponent, motionComponent>();
    for (auto entity : view) {
        auto& [position, direction, radians, speed] = view.get<motionComponent>(entity);
        auto& destination = view.get<enemyComponent>(entity).destination;
        if (direction.x == 0.f && direction.y == 0.f) {
            // set new destination
            // update direction
            destination.x = LO + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (HI_X - LO)));
            destination.y = LO + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (HI_Y - LO)));
            direction = normalize(sub(destination, position));
        } else if (position.x == destination.x && position.y == destination.y) {
            direction = { 0.f, 0.f };
        }
    }
}

void EnemyAISystem::set_target(entt::registry& registry)
{
    vec2 char_pos;
    auto character_view = registry.view<characterComponent, motionComponent>();
    for (auto character : character_view) {
        char_pos = character_view.get<motionComponent>(character).position;
    }

    auto enemy_view = registry.view<enemyComponent, motionComponent>();
    for (auto entity : enemy_view) {
        auto& target = enemy_view.get<enemyComponent>(entity).target;
        target = char_pos;
    }
}

void EnemyAISystem::set_rotation(entt::registry& registry)
{
    auto view = registry.view<enemyComponent, motionComponent>();
    for (auto entity : view) {
        auto& [position, direction, radians, speed] = view.get<motionComponent>(entity);
        auto& target = view.get<enemyComponent>(entity).target;
        radians = atan2(target.x - position.x, position.y - target.y);
    }
}

void EnemyAISystem::shoot(entt::registry& registry, float elapsed_ms, vector<Enemy>& m_enemies, vector<Projectile>& m_projectiles)
{
    auto enemies = registry.view<enemyComponent, motionComponent>();
    for (auto enemy : enemies) {
        auto& [position, direction, radians, speed] = enemies.get<motionComponent>(enemy);
        auto& [id, is_alive, shoot_delay_ms, destination, target] = enemies.get<enemyComponent>(enemy);
        if (is_alive && shoot_delay_ms < 0.f) {
            vec2 proj_direction = { target.x - position.x, target.y - position.y };
            bool enemy_alive = false;
            for (auto& m_enemy : m_enemies) {
                if (id == m_enemy.get_id()) {
                    Projectile projectile;
                    if (projectile.init(proj_counter)) {
                        vec2 face_pos = m_enemy.get_face_position();
                        m_projectiles.emplace_back(projectile);
                        makeProjectile(registry, proj_counter, face_pos, proj_direction, radians + 1.2);
                        proj_counter++;
                    } else {
                        fprintf(stderr, "Failed to spawn projectile");
                    }
                    enemy_alive = true;
                }
            }
            shoot_delay_ms = 3000.f;
            is_alive = enemy_alive;
        } else {
            shoot_delay_ms -= elapsed_ms;
        }
    }
}

void EnemyAISystem::destroy_dead_enemies(entt::registry& registry)
{
    auto enemies = registry.view<enemyComponent, motionComponent>();
    for (const auto enemy : enemies) {
        auto& is_alive = enemies.get<enemyComponent>(enemy).is_alive;
        if (!is_alive)
            registry.destroy(enemy);
    }
}
