#include "enemyAISystem.hpp"

float LO = 100.f;
float HI_X = 1100.f;
float HI_Y = 700.f;

void EnemyAISystem::update(entt::registry &registry, float elapsed_ms, vector<Enemy> &m_enemies)
{
    vec2 char_pos;
    auto character_view = registry.view<characterComponent, motionComponent>();
    for (auto character : character_view)
    {
        char_pos = character_view.get<motionComponent>(character).position;
    }

    auto enemies = registry.view<enemyComponent, motionComponent>();
    for (auto enemy : enemies)
    {
        auto &[id, health, enemy_type, is_alive, is_movable, shoot_cooldown, shoot_frequency, destination, target, es] = enemies.get<enemyComponent>(enemy);
        if (enemy_type == MINION)
        {
            for (auto &m_enemy : m_enemies)
            {
                if (id == m_enemy.get_id())
                {
                    bool on_sight = m_enemy.on_sight(target);
                    if (on_sight && !es.alert)
                    {
                        es.turn_sum = 0.f;
                        es.turn_step = 0.01f;
                        es.is_turn_complete = false;
                        es.alert = true;
                        es.enemy_on_sight = true;
                        es.alert_cooldown_ms = 25000;
                        m_enemy.alert();
                        es.line_of_sight = m_enemy.set_line(target, m_enemies);
                    }
                    else if (on_sight && es.alert)
                    {
                        es.alert_cooldown_ms = 25000;
                        es.turn_sum = 0.f;
                        es.turn_step = 0.01f;
                        es.is_turn_complete = false;
                        es.enemy_on_sight = true;
                        m_enemy.alert();
                        es.line_of_sight = m_enemy.set_line(target, m_enemies);
                    }
                    else if (!on_sight && es.alert)
                    {
                        es.alert_cooldown_ms -= elapsed_ms;
                        es.enemy_on_sight = false;
                        es.line_of_sight = false;
                        m_enemy.search();
                        m_enemy.unset_line();
                    }
                    if (es.alert_cooldown_ms <= 0)
                    {
                        es.alert = false;
                        m_enemy.idle();
                    }
                }
            }
        }
    }

    for (auto entity : enemies)
    {
        auto &[id, health, enemy_type, is_alive, is_movable, shoot_cooldown, shoot_frequency, destination, target, stateMachine] = enemies.get<enemyComponent>(entity);
        auto &[position, direction, radians, speed] = enemies.get<motionComponent>(entity);
        if (enemy_type == MINION)
        {
            if (stateMachine.alert)
            {
                if (stateMachine.enemy_on_sight)
                {
                    if (stateMachine.line_of_sight)
                    {
                        stateMachine.action = SHOOT;
                    }
                    else
                    {
                        stateMachine.action = PICK_DEST;
                        direction = {0.f, 0.f};
                    }
                }
                else
                {
                    if (stateMachine.is_turn_complete && stateMachine.action != TURN_TO_DEST && stateMachine.action != MOVE)
                    {
                        stateMachine.action = PICK_DEST;
                        direction = {0.f, 0.f};
                    }
                    else if (stateMachine.action != PICK_DEST && stateMachine.action != MOVE && stateMachine.action != TURN_TO_DEST)
                    {
                        stateMachine.action = TURN;
                    }
                }
            }
            else
            {
                // system.check_if_p_onsight
                // yes -> alert
                // no  -> remain idle
            }
        }
    }
}

void EnemyAISystem::set_direction(entt::registry &registry)
{
    auto view = registry.view<enemyComponent, motionComponent>();
    for (auto entity : view)
    {
        auto &[position, direction, radians, speed] = view.get<motionComponent>(entity);
        auto &[id, health, enemy_type, is_alive, is_movable, shoot_cooldown, shoot_frequency, destination, target, es] = view.get<enemyComponent>(entity);
        if (enemy_type == MINION)
        {
            if (direction.x == 0.f && direction.y == 0.f && es.action == PICK_DEST)
            {
                // set new destination
                // update direction
                destination.x = LO + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (HI_X - LO)));
                destination.y = LO + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (HI_Y - LO)));
                direction = normalize(sub(destination, position));
                es.action = TURN_TO_DEST;
                es.turn_target = atan2(destination.x - position.x, position.y - destination.y);
            }
            else if (position.x == destination.x && position.y == destination.y)
            {
                direction = {0.f, 0.f};
                es.action = TURN;
            }
        }
        else if (enemy_type == BOSS)
        {
            if (direction.x == 0.f && direction.y == 0.f)
            {
                // set new destination
                // update direction
                destination.x = position.x;
                destination.y = LO + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (HI_Y - LO)));
                direction = normalize(sub(destination, position));
            }
            else if (position.x == destination.x && position.y == destination.y)
            {
                direction = {0.f, 0.f};
            }
        }
    }
}

void EnemyAISystem::set_target(entt::registry &registry)
{
    vec2 char_pos;
    auto character_view = registry.view<characterComponent, motionComponent>();
    for (auto character : character_view)
    {
        char_pos = character_view.get<motionComponent>(character).position;
    }

    auto enemy_view = registry.view<enemyComponent, motionComponent>();
    for (auto entity : enemy_view)
    {
        auto &[id, health, enemy_type, is_alive, is_movable, shoot_cooldown, shoot_frequency, destination, target, es] = enemy_view.get<enemyComponent>(entity);
        target = char_pos;
    }
}

void EnemyAISystem::set_rotation(entt::registry &registry)
{
    auto view = registry.view<enemyComponent, motionComponent>();
    for (auto entity : view)
    {
        auto &[position, direction, radians, speed] = view.get<motionComponent>(entity);
        auto &[id, health, enemy_type, is_alive, is_movable, shoot_cooldown, shoot_frequency, destination, target, es] = view.get<enemyComponent>(entity);
        if (enemy_type == MINION)
        {
            if (es.enemy_on_sight)
                radians = atan2(target.x - position.x, position.y - target.y);
            else if (es.action == TURN)
            {
                radians += es.turn_step;
                es.turn_sum += es.turn_step;
                if (es.turn_sum > 0.8f)
                    es.turn_step = -0.01f;
                if (es.turn_sum < -0.8f)
                {
                    es.turn_step = 0.f;
                    es.turn_sum = 0.f;
                    es.is_turn_complete = true;
                }
            }
            else if (es.action == TURN_TO_DEST)
            {
                if (radians < es.turn_target)
                {
                    radians += 0.01f;
                }
                if (radians > es.turn_target)
                {
                    radians -= 0.01f;
                }
                if (fabs(radians - es.turn_target) < 0.02)
                {
                    radians = es.turn_target;
                    es.action = MOVE;
                }
            }
        }
        else if (enemy_type == BOSS)
        {
            radians = atan2(target.x - position.x, position.y - target.y);
        }
    }
}

void EnemyAISystem::shoot(entt::registry &registry, int enemy_type, vec2 proj_direction, vec2 face_pos, float radians, vector<Projectile> &m_projectiles)
{
    if (enemy_type == BOSS)
    {
        vec2 proj_addition = {-100.f, -100.f};
        for (int i = 0; i < 3; i++)
        {
            Projectile projectile;
            vec2 proj_direction_result = add(proj_direction, proj_addition);
            proj_addition = add(proj_addition, {100.f, 100.f});
            if (projectile.init(proj_counter))
            {
                m_projectiles.emplace_back(projectile);
                makeProjectile(registry, proj_counter, face_pos, proj_direction_result, radians + 1.2);
                proj_counter++;
            }
            else
            {
                fprintf(stderr, "Failed to spawn projectile");
            }
        }
    }
    else if (enemy_type == MINION)
    {
        Projectile projectile;
        if (projectile.init(proj_counter))
        {
            m_projectiles.emplace_back(projectile);
            makeProjectile(registry, proj_counter, face_pos, proj_direction, radians + 1.2);
            proj_counter++;
        }
        else
        {
            fprintf(stderr, "Failed to spawn projectile");
        }
    }
}

void EnemyAISystem::shoot_manager(entt::registry &registry, float elapsed_ms, vector<Enemy> &m_enemies, vector<Projectile> &m_projectiles)
{
    auto enemies = registry.view<enemyComponent, motionComponent>();
    for (auto enemy : enemies)
    {
        auto &[position, direction, radians, speed] = enemies.get<motionComponent>(enemy);
        auto &[id, health, enemy_type, is_alive, is_movable, shoot_cooldown, shoot_frequency, destination, target, es] = enemies.get<enemyComponent>(enemy);
        if (is_alive && shoot_cooldown < 0.f && (enemy_type == BOSS || es.action == SHOOT))
        {
            for (auto &m_enemy : m_enemies)
            {
                if (id == m_enemy.get_id())
                {
                    vec2 face_pos = m_enemy.get_face_position();
                    vec2 proj_direction = {target.x - face_pos.x, target.y - face_pos.y};
                    shoot(registry, enemy_type, proj_direction, face_pos, radians, m_projectiles);
                }
            }
            shoot_cooldown = shoot_frequency;
        }
        else
        {
            shoot_cooldown -= elapsed_ms;
        }
    }
}

void EnemyAISystem::destroy_dead_enemies(entt::registry &registry)
{
    auto enemies = registry.view<enemyComponent, motionComponent>();
    for (const auto enemy : enemies)
    {
        auto &is_alive = enemies.get<enemyComponent>(enemy).is_alive;
        if (!is_alive)
            registry.destroy(enemy);
    }
}
