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
    //-------------------------------------------------------------------------
    // GLFW / OGL Initialization
    // Core Opengl 3.
    glfwSetErrorCallback(glfw_err_cb);
    if (!glfwInit())
    {
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
    auto key_redirect = [](GLFWwindow *wnd, int _0, int _1, int _2, int _3) { ((World *)glfwGetWindowUserPointer(wnd))->on_key(wnd, _0, _1, _2, _3); };
    auto mouse_redirect = [](GLFWwindow *wnd, int _0, int _1, int _2) { ((World *)glfwGetWindowUserPointer(wnd))->on_mouse_key(wnd, _0, _1, _2); };
    auto cursor_pos_redirect = [](GLFWwindow *wnd, double _0, double _1) { ((World *)glfwGetWindowUserPointer(wnd))->on_mouse_move(wnd, _0, _1); };
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

    if (!soundSystem.init())
    {
        fprintf(stderr, "Failed to load sounds\n %s\n %s\n %s\n make sure the data directory is present",
                audio_path("music.wav"),
                audio_path("character_dead.wav"),
                audio_path("character_reflect.wav"));
    }

    soundSystem.play_background_music();

    fprintf(stderr, "Loaded music\n");

    makeCharacter(registry);
    makeShield(registry);
    makeMenu(registry);
    makeGround(registry, 999);
    makeLevel(registry);

    levelSystem.init_level(registry);
    m_wall_manager.init(levelSystem.get_wall_orientation(), m_walls, 1);
    fprintf(stderr, "factory done\n");

    debug = false;
    level = 0;

    return m_character.init() && m_background.init() && m_shield.init() && m_ground.init(999) && m_menu.init();
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
    m_wall_manager.destroy(m_walls);
    for (auto &projectile : m_projectiles)
        projectile.destroy();
    m_projectiles.clear();

    for (auto &enemy : m_enemies)
        enemy.destroy();
    m_enemies.clear();

    glfwDestroyWindow(m_window);
}

// Update our game world
bool World::update(float elapsed_ms)
{
    menuSystem.update(registry, m_menu);
    int temp_lvl = levelSystem.update(registry, elapsed_ms, &m_enemies, &m_projectiles);
    if (temp_lvl != level)
    {
        level = temp_lvl;
        m_ground.load_texture(level);
    }
    m_wall_manager.update(m_walls, levelSystem.getLevel(), levelSystem.get_wall_orientation());
    state = menuSystem.get_state(registry);
    debug = menuSystem.get_debug_mode(registry);

    if (state != STATE_PLAYING)
    {
        return false;
    }

    int w, h;
    glfwGetFramebufferSize(m_window, &w, &h);
    vec2 screen = {(float)w / m_screen_scale, (float)h / m_screen_scale};

    collisionSystem.update(registry, m_character, m_shield, m_enemies, m_projectiles, m_potion, m_points, m_walls);

    enemyAI.update(registry, elapsed_ms, m_enemies);
    enemyAI.set_direction(registry);
    enemyAI.set_target(registry);
    enemyAI.set_rotation(registry);
    enemyAI.shoot_manager(registry, elapsed_ms, m_enemies, m_projectiles);

    physicsSystem.sync(registry, elapsed_ms, m_walls);
    physicsSystem.update(registry, m_character, m_shield, m_enemies, m_projectiles, m_potion, m_ground);
    healthSystem.update(registry, m_enemies);

    if (levelSystem.should_spawn_minion(m_enemies.size()))
    {
        spawn_minion();
    }

    if (levelSystem.should_spawn_boss(registry))
    {
        spawn_boss();
        makePotion(registry, 1);
        m_potion.init(1);
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
    const float clear_color[3] = {0.16f, 0.07f, 0.05f};
    glClearColor(clear_color[0], clear_color[1], clear_color[2], 1.0);
    glClearDepth(1.f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Fake projection matrix, scales with respect to window coordinates
    // PS: 1.f / w in [1][1] is correct.. do you know why ? (:
    float left = 0.f;                         // *-0.5;
    float top = 0.f;                          // (float)h * -0.5;
    float right = (float)w / m_screen_scale;  // *0.5;
    float bottom = (float)h / m_screen_scale; // *0.5;

    float sx = 2.f / (right - left);
    float sy = 2.f / (top - bottom);
    float tx = -(right + left) / (right - left);
    float ty = -(top + bottom) / (top - bottom);
    mat3 projection_2D{{sx, 0.f, 0.f}, {0.f, sy, 0.f}, {tx, ty, 1.f}};

    // Drawing entities
    m_ground.draw(projection_2D);
    m_wall_manager.draw(m_walls, projection_2D);
    m_character.draw(projection_2D);
    m_shield.draw(projection_2D);
    m_potion.draw(projection_2D);

    for (auto &enemy : m_enemies)
        enemy.draw(projection_2D, debug);

    for (auto &projectile : m_projectiles)
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

// create a new minion
bool World::spawn_minion()
{
    Enemy enemy;
    int id = levelSystem.get_next_enemy_id();
    if (enemy.init(id))
    {
        m_enemies.emplace_back(enemy);
        vec2 pos = levelSystem.get_next_minion_pos();
        bool is_movable = levelSystem.get_next_minion_is_movable();
        makeMinion(registry, id, pos, is_movable);
        return true;
    }
    fprintf(stderr, "Failed to spawn enemy");
    return false;
}

// create a new enemy
bool World::spawn_boss()
{
    Enemy enemy;
    int id = levelSystem.get_next_enemy_id();
    if (enemy.init(id))
    {
        m_enemies.emplace_back(enemy);
        vec2 pos = levelSystem.get_next_boss_pos();
        bool is_movable = levelSystem.get_next_boss_is_movable();
        makeBoss(registry, id, pos, is_movable);
        return true;
    }
    fprintf(stderr, "Failed to spawn enemy");
    return false;
}

// On key callback
void World::on_key(GLFWwindow *, int key, int, int action, int mod)
{
    inputSystem.on_key(registry, key, action, mod);
}

// On key callback
void World::on_mouse_key(GLFWwindow *, int key, int action, int mod)
{
    inputSystem.on_mouse_key(registry, key, action, mod);
}

void World::on_mouse_move(GLFWwindow *window, double xpos, double ypos)
{
    inputSystem.on_mouse_move(registry, xpos, ypos);
}