// Header
#include "world.hpp"

// stlib
#include <cassert>
#include <sstream>
#include <string.h>

// Same as static in c, local to compilation unit
namespace {
    // change these numbers for minimal difficulty control
    const size_t MAX_ENEMIES = 35;
    const size_t ENEMY_SPAWN_DELAY_MS = 200;

    namespace {
        void glfw_err_cb(int error, const char* desc)
        {
            fprintf(stderr, "%d: %s", error, desc);
        }
    } // namespace
} // namespace

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
    m_window = glfwCreateWindow((int)screen.x, (int)screen.y, "Salmon Game Assignment", nullptr, nullptr);
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
    auto cursor_pos_redirect = [](GLFWwindow* wnd, double _0, double _1) { ((World*)glfwGetWindowUserPointer(wnd))->on_mouse_move(wnd, _0, _1); };
    glfwSetKeyCallback(m_window, key_redirect);
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

    //-------------------------------------------------------------------------
    // Loading music and sounds
    if (SDL_Init(SDL_INIT_AUDIO) < 0) {
        fprintf(stderr, "Failed to initialize SDL Audio");
        return false;
    }

    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) == -1) {
        fprintf(stderr, "Failed to open audio device");
        return false;
    }

    m_background_music = Mix_LoadMUS(audio_path("music.wav"));
    m_salmon_dead_sound = Mix_LoadWAV(audio_path("salmon_dead.wav"));
    m_salmon_eat_sound = Mix_LoadWAV(audio_path("salmon_eat.wav"));

    if (m_background_music == nullptr || m_salmon_dead_sound == nullptr || m_salmon_eat_sound == nullptr) {
        fprintf(stderr, "Failed to load sounds\n %s\n %s\n %s\n make sure the data directory is present",
            audio_path("music.wav"),
            audio_path("salmon_dead.wav"),
            audio_path("salmon_eat.wav"));
        return false;
    }

    // Playing background music indefinitely
    Mix_PlayMusic(m_background_music, -1);

    fprintf(stderr, "Loaded music\n");

    m_current_speed = 1.f;

    enemy_number = 0;

    makeCharacter(registry);
    makeShield(registry);
    spawn_enemy(enemy_number);
    fprintf(stderr, "factory done\n");

    return m_character.init()
        && m_background.init()
        && m_shield.init()
        && m_potion.init();
}

// Releases all the associated resources
void World::destroy()
{
    glDeleteFramebuffers(1, &m_frame_buffer);

    if (m_background_music != nullptr)
        Mix_FreeMusic(m_background_music);
    if (m_salmon_dead_sound != nullptr)
        Mix_FreeChunk(m_salmon_dead_sound);
    if (m_salmon_eat_sound != nullptr)
        Mix_FreeChunk(m_salmon_eat_sound);

    Mix_CloseAudio();

    m_character.destroy();
    m_potion.destroy();
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
    int w, h;
    glfwGetFramebufferSize(m_window, &w, &h);
    vec2 screen = { (float)w / m_screen_scale, (float)h / m_screen_scale };

    // Checking Salmon - Turtle collisions
    if (m_character.collides_with(m_potion) && m_potion.is_alive()) {
        m_potion.destroy();
        physicsSystem.setShieldScaleMultiplier(registry, 2.0f, 1.0f);
    }

    int i = 0;
    for (auto& projectile : m_projectiles) {
        int j = 0;
        bool hits_enemy = false;
        for (auto& enemy: m_enemies)
        {
            if (enemy.collides_with(projectile))
            {
                enemy.kill();
                enemy_number--;
                m_enemies.erase(m_enemies.begin() + j);
                j--;
                hits_enemy = true;
                break;
            }
            j++;
        }
        if (m_character.collides_with(projectile) && !hits_enemy) {
            physicsSystem.setCharacterUnmovable(registry);
            m_projectiles.erase(m_projectiles.begin() + i);
            break;
        }
        if (hits_enemy)
        {   
            m_projectiles.erase(m_projectiles.begin() + i);
            i--;
        }
        i++;
    }

    for (auto& projectile : m_projectiles) {
        if (m_shield.collides_with(projectile)) {
            vec2 shieldDirection = m_shield.getDirection();
            vec2 projectileDirection = projectile.getDirection();

            vec2 reflection = sub(
                projectileDirection,
                mul(mul(shieldDirection, 2.f), dot(shieldDirection, shieldDirection)));

            projectile.setDirection(reflection);
            continue;
        }
    }


    enemyAI.set_direction(registry);
    enemyAI.set_target(registry);
    enemyAI.set_rotation(registry);
    // Updating all entities, making the turtle and fish
    // faster based on current.
    // In a pure ECS engine we would classify entities by their bitmap tags during the update loop
    // rather than by their class.

    m_potion.update(elapsed_ms);
    physicsSystem.sync(registry, elapsed_ms);
    physicsSystem.update(registry, m_character, m_shield, m_enemies);
    for (auto& projectile : m_projectiles)
        projectile.update(elapsed_ms * m_current_speed);

    // Removing out of screen projectiles
    auto projectile_it = m_projectiles.begin();
    while (projectile_it != m_projectiles.end()) {
        float w = projectile_it->get_bounding_box().x / 2;
        float h = projectile_it->get_bounding_box().y / 2;
        if (projectile_it->get_position().x + w < 0.f || projectile_it->get_position().x - w > 1200.f || projectile_it->get_position().y + h < 0.f || projectile_it->get_position().y - h > 850.f) {
            projectile_it = m_projectiles.erase(projectile_it);
            continue;
        }
        ++projectile_it;
    }

    enemyAI.shoot(registry, elapsed_ms, m_enemies, m_projectiles);

    m_next_enemy_spawn -= elapsed_ms;
    if (enemy_number<=5 && m_enemies.size() <= MAX_ENEMIES && m_next_enemy_spawn < 0.f)
    {
        if(spawn_enemy(enemy_number))
            m_next_enemy_spawn = ENEMY_SPAWN_DELAY_MS;
        else
            fprintf(stderr, "%s\n", "couldn't spawn new enemy");
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
    if (m_potion.is_alive())
        m_potion.draw(projection_2D);
    // if (m_enemy.is_alive())
    //     m_enemy.draw(projection_2D);
    for (auto& enemy : m_enemies)
        enemy.draw(projection_2D);
    for (auto& projectile : m_projectiles)
        projectile.draw(projection_2D);

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
    return glfwWindowShouldClose(m_window);
}

// create a new enemy
bool World::spawn_enemy(int &id)
{
    Enemy enemy;
    if (enemy.init(id)) {
        m_enemies.emplace_back(enemy);
        makeEnemy(registry, id);
        id++;
        return true;
    }
    fprintf(stderr, "Failed to spawn enemy");
    return false;
}

// On key callback
void World::on_key(GLFWwindow*, int key, int, int action, int mod)
{
    // Resetting game
    if (action == GLFW_RELEASE && key == GLFW_KEY_R) {
        int w, h;
        glfwGetWindowSize(m_window, &w, &h);
        m_projectiles.clear();
        m_background.reset_salmon_dead_time();
        m_current_speed = 1.f;
    }

    inputSystem.on_key(registry, key, action, mod);

    // Control the current speed with `<` `>`
    if (action == GLFW_RELEASE && (mod & GLFW_MOD_SHIFT) && key == GLFW_KEY_COMMA)
        m_current_speed -= 0.1f;
    if (action == GLFW_RELEASE && (mod & GLFW_MOD_SHIFT) && key == GLFW_KEY_PERIOD)
        m_current_speed += 0.1f;

    m_current_speed = fmax(0.f, m_current_speed);
}

void World::on_mouse_move(GLFWwindow* window, double xpos, double ypos)
{
    inputSystem.on_mouse(registry, xpos, ypos);
}

// Calculates the length of a vec2 vector
float World::lengthVec2(vec2 v)
{
    return sqrt(pow(v.x, 2.f) + pow(v.y, 2.f));
}
