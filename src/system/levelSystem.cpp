#include "levelSystem.hpp"

bool LevelSystem::init_level(entt::registry &registry)
{
    reset_enemy(registry);

    auto level = registry.view<levelComponent>();

    for (auto entity : level)
    {
        auto &[minion_killed, boss_killed, zombie_killed] = level.get(entity);

        // shared info
        minion_count = 0;
        minion_killed = 0;
        zombie_count = 0;
        zombie_killed = 0;
        boss_count = 0;
        boss_killed = 0;
        enemy_killed = 0;

        switch (lvl_num)
        {
        case 1:
            // minion info
            minion_num = 1;
            minion_max_on_screen = 1;
            for (vec2 pos : minion_level1_pos_array)
            {
                minion_init_pos.push_back(pos);
            }
            for (bool is_movable : minion_level1_movable_array)
            {
                minion_is_movable.push_back(is_movable);
            }

            // zombie information
            zombie_num = 1;
            zombie_max_on_screen = 1;
            for (vec2 pos : zombie_level1_pos_array)
            {
                zombie_init_pos.push_back(pos);
            }
            for (bool is_movable : zombie_level1_movable_array)
            {
                zombie_is_movable.push_back(is_movable);
            }

            // boss info
            boss_num = 0;
            boss_max_on_screen = 1;

            // global info
            enemy_spawn_delay = 50.f;
            break;
        case 2:
            // minion info
            minion_num = 3;
            minion_max_on_screen = 3;
            for (vec2 pos : minion_level2_pos_array)
            {
                minion_init_pos.push_back(pos);
            }
            for (bool is_movable : minion_level2_movable_array)
            {
                minion_is_movable.push_back(is_movable);
            }

            // zombie information
            zombie_num = 3;
            zombie_max_on_screen = 3;
            for (vec2 pos : zombie_level2_pos_array)
            {
                zombie_init_pos.push_back(pos);
            }
            for (bool is_movable : zombie_level2_movable_array)
            {
                zombie_is_movable.push_back(is_movable);
            }

            // boss info
            boss_num = 0;
            boss_max_on_screen = 1;

            // global info
            enemy_spawn_delay = 0.f;
            break;
        case 3:
            // minion info
            minion_num = 3;
            minion_max_on_screen = 3;
            for (vec2 pos : minion_level3_pos_array)
            {
                minion_init_pos.push_back(pos);
            }
            for (bool is_movable : minion_level3_movable_array)
            {
                minion_is_movable.push_back(is_movable);
            }

            // zombie information
            zombie_num = 5;
            zombie_max_on_screen = 5;
            for (vec2 pos : zombie_level3_pos_array)
            {
                zombie_init_pos.push_back(pos);
            }
            for (bool is_movable : zombie_level3_movable_array)
            {
                zombie_is_movable.push_back(is_movable);
            }
            // boss info
            boss_num = 0;
            boss_max_on_screen = 1;

            // global info
            enemy_spawn_delay = 0.f;
            break;
        case 4:
            // minion info
            minion_num = 0;
            minion_max_on_screen = 3;

            // zombie information
            zombie_num = 36;
            zombie_max_on_screen = 36;
            for (vec2 pos : zombie_level4_pos_array)
            {
                zombie_init_pos.push_back(pos);
            }
            for (bool is_movable : zombie_level4_movable_array)
            {
                zombie_is_movable.push_back(is_movable);
            }

            // boss info
            boss_num = 1;
            boss_max_on_screen = 1;
            for (vec2 pos : boss_level4_pos_array)
            {
                boss_init_pos.push_back(pos);
            }
            for (bool is_movable : boss_level4_movable_array)
            {
                boss_is_movable.push_back(is_movable);
            }

            // global info
            enemy_spawn_delay = 1000.f;
            break;
        default:
            return false;
        }

        // shared info
        enemy_total = minion_num + boss_num + zombie_num;
        next_enemy_spawn_counter = 0.f;
    }

    return true;
}


bool LevelSystem::check_all_enemies_are_dead(entt::registry &registry)
{
    auto level = registry.view<levelComponent>();
    for (auto entity : level)
    {
        auto &[minion_killed, boss_killed, zombie_killed] = level.get(entity);
        enemy_killed = minion_killed + boss_killed + zombie_killed;
        return enemy_killed >= enemy_total;
    }
    return false;
}

int LevelSystem::update(entt::registry &registry, float elapsed_ms, Character *m_character, vector<Enemy> *m_enemies, vector<Zombie> *m_zombies, vector<Projectile> *m_projectiles, Redzone *m_redzone)
{
    next_enemy_spawn_counter -= elapsed_ms;

    auto level = registry.view<levelComponent>();

    for (auto entity : level)
    {
        auto &[minion_killed, boss_killed, zombie_killed] = level.get(entity);
        enemy_killed = minion_killed + boss_killed + zombie_killed;

        if (enemy_killed >= enemy_total)
        {
            auto viewCharacter = registry.view<characterComponent, motionComponent>();
            for (auto character : viewCharacter)
            {
                auto &position = viewCharacter.get<motionComponent>(character).position;
                if (position.x > 1100 && position.y > 130 && position.y < 210)
                {
                    position = {30, 150};
                    lvl_num++;
                    if (!init_level(registry))
                    {
                        menuSystem.sync(registry, STATE_WIN);
                        lvl_num--;
                        return lvl_num;
                    }
                }
            }
        }
    }

    auto view = registry.view<inputKeyboard, inputMouse>();
    for (auto entity : view)
    {
        auto &resetKeyPressed = view.get<inputKeyboard>(entity).resetKeyPressed;
        auto &saveKeyPressed = view.get<inputKeyboard>(entity).saveKeyPressed;
        auto &loadKeyPressed = view.get<inputKeyboard>(entity).loadKeyPressed;

        if (resetKeyPressed)
        {
            m_enemies->clear();
            m_zombies->clear();
            m_projectiles->clear();
            m_redzone->set_position({-100.f, -100.f});
            m_character->restart_health();
            lvl_num = 1;
            init_level(registry);
        }
        if (saveKeyPressed)
        {
            remove("level.txt");
            ofstream outfile("level.txt");
            fprintf(stderr, "Saving level from file...\n");
            outfile << lvl_num << endl;
            outfile.close();
            saveKeyPressed = false;
        }
        if (loadKeyPressed)
        {
            fprintf(stderr, "Reloading level from file...\n");
            ifstream in;
            in.open("level.txt");
            if (!in)
            {
                fprintf(stderr, "Error: Can’t open the file named level.txt.\n");
            }
            else
            {
                in >> lvl_num;
            }
            in.close();
            loadKeyPressed = false;
            m_enemies->clear();
            m_zombies->clear();
            m_projectiles->clear();
            init_level(registry);
        }
    }

    return lvl_num;
}

// Minion functions
vec2 LevelSystem::get_next_minion_pos()
{
    vec2 result = minion_init_pos[0];
    auto init_pos = minion_init_pos.begin();
    minion_init_pos.erase(init_pos);
    return result;
}

bool LevelSystem::get_next_minion_is_movable()
{
    bool result = minion_is_movable[0];
    auto init_pos = minion_is_movable.begin();
    minion_is_movable.erase(init_pos);
    return result;
}

vec2 LevelSystem::get_next_zombie_pos()
{
    vec2 result = zombie_init_pos[0];
    auto init_pos = zombie_init_pos.begin();
    zombie_init_pos.erase(init_pos);
    return result;
}

bool LevelSystem::should_spawn_minion(int enemy_size)
{
    if (enemy_size < minion_max_on_screen && next_enemy_spawn_counter < 0.f && minion_count < minion_num)
    {
        minion_count++;
        next_enemy_spawn_counter = enemy_spawn_delay;
        return true;
    }

    return false;
}

bool LevelSystem::should_spawn_zombie(int zombies_size)
{
    if (zombies_size < zombie_max_on_screen && next_enemy_spawn_counter < 0.f && zombie_count < zombie_num)
    {
        zombie_count++;
        next_enemy_spawn_counter = enemy_spawn_delay;
        return true;
    }
    return false;
}

// Boss functions
vec2 LevelSystem::get_next_boss_pos()
{
    vec2 result = boss_init_pos[0];
    auto init_pos = boss_init_pos.begin();
    boss_init_pos.erase(init_pos);
    return result;
}

bool LevelSystem::get_next_boss_is_movable()
{
    bool result = boss_is_movable[0];
    auto init_pos = boss_is_movable.begin();
    boss_is_movable.erase(init_pos);
    return result;
}

void LevelSystem::reset_enemy(entt::registry &registry)
{
    minion_init_pos.clear();
    minion_is_movable.clear();
    boss_init_pos.clear();
    boss_is_movable.clear();
    wallVector.clear();

    auto viewEnemy = registry.view<enemyComponent, physicsScaleComponent, motionComponent>();

    for (auto entity : viewEnemy)
    {
        auto &is_alive = viewEnemy.get<enemyComponent>(entity).is_alive;
        is_alive = false;
    }

    auto level = registry.view<levelComponent>();

    for (auto entity : level)
    {
        auto &[minion_killed, boss_killed, zombie_killed] = level.get(entity);
        minion_killed = 0;
        boss_killed = 0;
        zombie_killed = 0;
    }

    auto menu = registry.view<menuComponent>();
    for (auto entity : menu)
    {
        auto &state = menu.get(entity).state;
    }
}

bool LevelSystem::should_spawn_boss(entt::registry &registry)
{
    if (enemy_killed == minion_num && boss_count < boss_num)
    {
        auto view = registry.view<potionComponent, physicsScaleComponent>();
        for (auto entity : view)
        {
            auto &id = view.get<potionComponent>(entity).id;
            auto &is_consumed = view.get<potionComponent>(entity).is_consumed;
            auto &scale = view.get<physicsScaleComponent>(entity).scale;
            scale = {0.07f, 0.07f};
            is_consumed = false;
        }
        boss_count++;
        return true;
    }

    return false;
}

// Enemy shared functions
int LevelSystem::get_next_enemy_id()
{
    return enemy_id++;
}

void LevelSystem::increment_enemy_killed(entt::registry &registry)
{
    auto level = registry.view<levelComponent>();

    for (auto entity : level)
    {
        auto &[minion_killed, boss_killed, zombie_killed] = level.get(entity);
        minion_killed++;
    }
}

vector<vec2> LevelSystem::get_wall_orientation()
{
    wallVector.clear();
    for (vec2 pos : surrounding_wall_pos_array)
    {
        wallVector.push_back(pos);
    }

    switch (lvl_num)
    {
    case 1:
        for (vec2 pos : wall_level1_pos_array)
        {
            wallVector.push_back(pos);
        }
        break;
    case 2:
        for (vec2 pos : wall_level2_pos_array)
        {
            wallVector.push_back(pos);
        }
        break;
    case 3:
        for (vec2 pos : wall_level3_pos_array)
        {
            wallVector.push_back(pos);
        }
        break;
    case 4:
        for (vec2 pos : wall_level4_pos_array)
        {
            wallVector.push_back(pos);
        }
        break;
    }
    return wallVector;
}

int LevelSystem::getLevel()
{
    return lvl_num;
}
