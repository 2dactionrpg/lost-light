#include "levelSystem.hpp"

void LevelSystem::load_level_info(entt::registry& registry, int& m_lvl_num, int& m_minion_max_on_screen,
    int& m_boss_max_on_screen, int& m_minion_num, int& m_boss_num, int& m_enemy_total, int& m_enemy_killed)
{
    auto level = registry.view<levelComponent>();

    for (auto entity : level) {
        auto& [minion_killed, boss_killed] = level.get(entity);
        minion_killed = m_enemy_killed;
    }
    m_lvl_num = lvl_num;
    m_minion_max_on_screen = minion_max_on_screen;
    m_boss_max_on_screen = boss_max_on_screen;
    m_minion_num = minion_num;
    m_boss_num = boss_num;
    m_enemy_total = enemy_total;
}

void LevelSystem::update(entt::registry& registry, int& m_enemy_killed)
{
    auto level = registry.view<levelComponent>();

    for (auto entity : level) {
        auto& [minion_killed, boss_killed] = level.get(entity);
        m_enemy_killed = minion_killed;
    }
}

void LevelSystem::increment_enemy_killed(entt::registry& registry)
{
    auto level = registry.view<levelComponent>();

    for (auto entity : level) {
        auto& [minion_killed, boss_killed] = level.get(entity);
        minion_killed++;
    }
}

bool LevelSystem::init_level(entt::registry& registry, int m_lvl_num)
{
    auto level = registry.view<levelComponent>();

    for (auto entity : level) {
        auto& [minion_killed, boss_killed] = level.get(entity);
        switch (m_lvl_num) {
        case 1:
            lvl_num = 1;
            minion_max_on_screen = 3;
            boss_max_on_screen = 1;
            minion_num = 3;
            boss_num = 1;
            enemy_total = minion_num + boss_num;
            minion_killed = 0;
            boss_killed = 0;
            minion_init_pos.push_back({ 1000.f, 1000.f });
            minion_init_pos.push_back({ 500.f, 500.f });
            minion_init_pos.push_back({ 0.f, 0.f });
            boss_init_pos.push_back({ 400.f, 400.f });
            break;
        case 2:
            break;
        case 3:
            break;
        case 4:
            break;
        case 5:
            break;
        }
    }

    return true;
}

vec2 LevelSystem::get_next_minion_pos()
{
    vec2 result = minion_init_pos[0];
    auto init_pos = minion_init_pos.begin();
    minion_init_pos.erase(init_pos);
    return result;
}

vec2 LevelSystem::get_next_boss_pos()
{
    vec2 result = boss_init_pos[0];
    auto init_pos = boss_init_pos.begin();
    boss_init_pos.erase(init_pos);
    return result;
}