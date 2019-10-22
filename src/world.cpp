// Header
#include "world.hpp"

// stlib
#include <cassert>
#include <sstream>
#include <string.h>

World::World()
    : m_points(0)
{
    // Seeding rng with random device
    m_rng = std::default_random_engine(std::random_device()());
}

World::~World()
{
}

// World initialization
bool World::init(vec2 screen)
{
    enemiesCount = 0;
    enemiesKilled = 0;
    bossCount = 0;
    //-------------------------------------------------------------------------
    // GLFW / OGL Initialization
    // Core Opengl 3.
    glfwSetErrorCallback(glfw_err_cb);
    if (!glfwInit()) {
        fprintf(stderr, "Failed to initialize GLFW");
        return false;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, 1);
#if __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
    glfwWindowHint(GLFW_RESIZABLE, 0);
    m_window = glfwCreateWindow((int)screen.x, (int)screen.y, "Lost Light", nullptr, nullptr);
    if (m_window == nullptr)
        return false;

    glfwMakeContextCurrent(m_window);
    glfwSwapInterval(1); // vsync

    // Load OpenGL function pointers
    gl3w_init();

    // Setting callbacks to member functions (that's why the redirect is needed)
    // Input is handled using GLFW, for more info see
    glfwSetWindowUserPointer(m_window, this);
    auto key_redirect = [](GLFWwindow* wnd, int _0, int _1, int _2, int _3) { ((World*)glfwGetWindowUserPointer(wnd))->on_key(wnd, _0, _1, _2, _3); };
    auto mouse_redirect = [](GLFWwindow* wnd, int _0, int _1, int _2) { ((World*)glfwGetWindowUserPointer(wnd))->on_mouse_key(wnd, _0, _1, _2); };
    auto cursor_pos_redirect = [](GLFWwindow* wnd, double _0, double _1) { ((World*)glfwGetWindowUserPointer(wnd))->on_mouse_move(wnd, _0, _1); };
    glfwSetKeyCallback(m_window, key_redirect);
    glfwSetMouseButtonCallback(m_window, mouse_redirect);
    glfwSetCursorPosCallback(m_window, cursor_pos_redirect);

    // Create a frame buffer
    m_frame_buffer = 0;
    glGenFramebuffers(1, &m_frame_buffer);
    glBindFramebuffer(GL_FRAMEBUFFER, m_frame_buffer);

    // For some high DPI displays (ex. Retina Display on Macbooks)
    int fb_width, fb_height;
    glfwGetFramebufferSize(m_window, &fb_width, &fb_height);
    m_screen_scale = static_cast<float>(fb_width) / screen.x;

    // Initialize the screen texture
    m_screen_tex.create_from_screen(m_window);

    if (!soundSystem.init()) {
        fprintf(stderr, "Failed to load sounds\n %s\n %s\n %s\n make sure the data directory is present",
            audio_path("music.wav"),
            audio_path("character_dead.wav"),
            audio_path("character_reflect.wav"));
    }

    soundSystem.play_background_music();

    fprintf(stderr, "Loaded music\n");

    enemy_number = 0;

    m_next_enemy_spawn = ENEMY_SPAWN_DELAY_MS;

    makeCharacter(registry);
    makeShield(registry);
    makePotion(registry, 1);
    makeMenu(registry);

    spawn_enemy(enemy_number);
    m_next_enemy_spawn = ENEMY_SPAWN_DELAY_MS;
    fprintf(stderr, "factory done\n");

    return m_character.init()
        && m_background.init()
        && m_shield.init()
        && m_potion.init(1)
        && m_menu.init();
}

// Releases all the associated resources
void World::destroy()
{
    glDeleteFramebuffers(1, &m_frame_buffer);

    soundSystem.destroy();

    m_character.destroy();
    m_potion.destroy();
    m_menu.destroy();
    m_shield.destroy();
    for (auto& projectile : m_projectiles)
        projectile.destroy();
    m_projectiles.clear();

    for (auto& enemy : m_enemies)
        enemy.destroy();
    m_enemies.clear();

    glfwDestroyWindow(m_window);
}

// Update our game world
bool World::update(float elapsed_ms)
{
    menuSystem.update(registry, m_menu);

    state = menuSystem.get_state(registry);
    if (state != STATE_PLAYING) {
        return false;
    }

    if (enemiesKilled >= ENEMIES_THRESHOLD_1 + MAX_BOSS_COUNT) {
        menuSystem.sync(registry, STATE_WIN);
        return false;
    }

    int w, h;
    glfwGetFramebufferSize(m_window, &w, &h);
    vec2 screen = { (float)w / m_screen_scale, (float)h / m_screen_scale };

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
            m_projectiles.erase(projectile_it);
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
            m_projectiles.erase(projectile_it);
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
            m_projectiles.erase(projectile_it);
        } else {
            ++projectile_it;
        }
    }

    enemyAI.set_direction(registry);
    enemyAI.set_target(registry);
    enemyAI.set_rotation(registry);
    enemyAI.shoot(registry, elapsed_ms, m_enemies, m_projectiles);

    physicsSystem.sync(registry, elapsed_ms);
    physicsSystem.update(registry, m_character, m_shield, m_enemies, m_projectiles, m_potion);
    healthSystem.update(registry, m_enemies, enemiesKilled);

    m_next_enemy_spawn -= elapsed_ms;
    if (m_enemies.size() < MAX_ENEMIES && m_next_enemy_spawn < 0.f && enemiesCount < ENEMIES_THRESHOLD_1) {
        if (spawn_enemy(enemy_number))
            m_next_enemy_spawn = ENEMY_SPAWN_DELAY_MS;
        else
            fprintf(stderr, "%s\n", "couldn't spawn new enemy");
    }

    if (enemiesKilled == ENEMIES_THRESHOLD_1 && bossCount < MAX_BOSS_COUNT) {
        spawn_boss(enemy_number);
        bossCount++;
    }

    enemyAI.destroy_dead_enemies(registry);

    return true;
}

// Render our game world
void World::draw()
{
    // Clearing error buffer
    gl_flush_errors();

    // Getting size of window
    int w, h;
    glfwGetFramebufferSize(m_window, &w, &h);

    // Updating window title with points
    std::stringstream title_ss;
    title_ss << "Points: " << m_points;
    glfwSetWindowTitle(m_window, title_ss.str().c_str());

    /////////////////////////////////////
    // First render to the custom framebuffer
    glBindFramebuffer(GL_FRAMEBUFFER, m_frame_buffer);

    // Clearing backbuffer
    glViewport(0, 0, w, h);
    glDepthRange(0.00001, 10);
    const float clear_color[3] = { 0.16f, 0.07f, 0.05f };
    glClearColor(clear_color[0], clear_color[1], clear_color[2], 1.0);
    glClearDepth(1.f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Fake projection matrix, scales with respect to window coordinates
    // PS: 1.f / w in [1][1] is correct.. do you know why ? (:
    float left = 0.f; // *-0.5;
    float top = 0.f; // (float)h * -0.5;
    float right = (float)w / m_screen_scale; // *0.5;
    float bottom = (float)h / m_screen_scale; // *0.5;

    float sx = 2.f / (right - left);
    float sy = 2.f / (top - bottom);
    float tx = -(right + left) / (right - left);
    float ty = -(top + bottom) / (top - bottom);
    mat3 projection_2D { { sx, 0.f, 0.f }, { 0.f, sy, 0.f }, { tx, ty, 1.f } };

    // Drawing entities
    m_character.draw(projection_2D);
    m_shield.draw(projection_2D);
    m_potion.draw(projection_2D);
    // if (m_enemy.is_alive())
    //     m_enemy.draw(projection_2D);
    for (auto& enemy : m_enemies)
        enemy.draw(projection_2D);
    for (auto& projectile : m_projectiles)
        projectile.draw(projection_2D);
    m_menu.draw(projection_2D);

    /////////////////////
    // Truely render to the screen
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // Clearing backbuffer
    glViewport(0, 0, w, h);
    glDepthRange(0, 10);
    glClearColor(0, 0, 0, 1.0);
    glClearDepth(1.f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Bind our texture in Texture Unit 0
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_screen_tex.id);

    m_background.draw(projection_2D);

    //////////////////
    // Presenting
    glfwSwapBuffers(m_window);
}

// Should the game be over ?
bool World::is_over() const
{
    return glfwWindowShouldClose(m_window) || state == STATE_TERMINATE;
}

// create a new enemy
bool World::spawn_enemy(int& id)
{
    Enemy enemy;
    if (enemy.init(id)) {
        m_enemies.emplace_back(enemy);
        makeEnemy(registry, id);
        id++;
        enemiesCount++;
        return true;
    }
    fprintf(stderr, "Failed to spawn enemy");
    return false;
}

// create a new enemy
bool World::spawn_boss(int& id)
{
    Enemy enemy;
    if (enemy.init(id)) {
        m_enemies.emplace_back(enemy);
        makeBoss(registry, id);
        id++;
        return true;
    }
    fprintf(stderr, "Failed to spawn enemy");
    return false;
}

// On key callback
void World::on_key(GLFWwindow*, int key, int, int action, int mod)
{
    inputSystem.on_key(registry, key, action, mod);
}

// On key callback
void World::on_mouse_key(GLFWwindow*, int key, int action, int mod)
{
    inputSystem.on_mouse_key(registry, key, action, mod);
}

void World::on_mouse_move(GLFWwindow* window, double xpos, double ypos)
{
    inputSystem.on_mouse_move(registry, xpos, ypos);
}
