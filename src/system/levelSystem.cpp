#include "levelSystem.hpp"

bool LevelSystem::init_level(entt::registry &registry)
{
    reset_enemy(registry);

    auto level = registry.view<levelComponent>();

    for (auto entity : level)
    {
        auto &[minion_killed, boss_killed] = level.get(entity);

        // shared info
        minion_count = 0;
        minion_killed = 0;
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
        enemy_total = minion_num + boss_num;
        next_enemy_spawn_counter = 0.f;
    }

    return true;
}

int LevelSystem::update(entt::registry &registry, float elapsed_ms, vector<Enemy> *m_enemies, vector<Projectile> *m_projectiles)
{
    next_enemy_spawn_counter -= elapsed_ms;

    auto level = registry.view<levelComponent>();

    for (auto entity : level)
    {
        auto &[minion_killed, boss_killed] = level.get(entity);
        enemy_killed = minion_killed + boss_killed;

        if (enemy_killed >= enemy_total)
        {
            lvl_num++;
            if (!init_level(registry))
            {
                menuSystem.sync(registry, STATE_WIN);
                lvl_num--;
                return lvl_num;
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
            m_projectiles->clear();
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
                printf("%d", lvl_num);
            }
            in.close();
            loadKeyPressed = false;
            m_enemies->clear();
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
        auto &[minion_killed, boss_killed] = level.get(entity);
        minion_killed = 0;
        boss_killed = 0;
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
        auto &[minion_killed, boss_killed] = level.get(entity);
        minion_killed++;
    }
}

vector<vec2> LevelSystem::get_wall_orientation()
{
    wallVector.clear();

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
