#pragma once

// internal
#include "character.hpp"
#include "common.hpp"
#include "data.hpp"
#include "enemy.hpp"
#include "menu.hpp"
#include "shield.hpp"
#include "wall.hpp"
#include "wall_manager.hpp"

#include "./system/collisionSystem.hpp"
#include "./system/enemyAISystem.hpp"
#include "./system/healthSystem.hpp"
#include "./system/inputSystem.hpp"
#include "./system/levelSystem.hpp"
#include "./system/menuSystem.hpp"
#include "./system/physicsSystem.hpp"
#include "./system/soundSystem.hpp"
#include "background.hpp"
#include "factory.hpp"
#include "ground.hpp"
#include "potion.hpp"
#include "projectile.hpp"

// entt
#include <entt/entity/registry.hpp>

// stlib
#include <random>
#include <vector>

// Same as static in c, local to compilation unit
namespace
{
namespace
{
void glfw_err_cb(int error, const char *desc)
{
    fprintf(stderr, "%d: %s", error, desc);
}
} // namespace
} // namespace

// Container for all our entities and game logic. Individual rendering / update is
// deferred to the relative update() methods
class World
{
public:
    World();
    ~World();

    // Creates a window, sets up events and begins the game
    bool init(vec2 screen);

    // Releases all associated resources
    void destroy();

    // Steps the game ahead by ms milliseconds
    bool update(float ms);

    // Renders our scene
    void draw();

    // Should the game be over ?
    bool is_over() const;

    int state;

    int level;

    bool debug;

private:
    // generate enemies
    bool spawn_minion();
    bool spawn_boss();

    // !!! INPUT CALLBACK FUNCTIONS
    void on_key(GLFWwindow *, int key, int, int action, int mod);
    void on_mouse_move(GLFWwindow *window, double xpos, double ypos);
    void on_mouse_key(GLFWwindow *, int key, int action, int mod);

    // Window handle
    GLFWwindow *m_window;
    float m_screen_scale; // Screen to pixel coordinates scale factor

    // Screen texture
    // The draw loop first renders to this texture, then it is used for the background shader
    GLuint m_frame_buffer;
    Texture m_screen_tex;

    // Background effect
    Background m_background;

    // Number of fish eaten by the character, displayed in the window title
    unsigned int m_points;

    // Game entities
    Character m_character;
    Menu m_menu;
    Shield m_shield;
    Potion m_potion;
    Ground m_ground;

    std::vector<Projectile> m_projectiles;
    std::vector<Enemy> m_enemies;
    WallManager m_wall_manager;
    std::vector<Wall> m_walls;

    // C++ rng
    std::default_random_engine m_rng;
    std::uniform_real_distribution<float> m_dist; // default 0..1

    InputSystem inputSystem;
    PhysicsSystem physicsSystem;
    EnemyAISystem enemyAI;
    HealthSystem healthSystem;
    MenuSystem menuSystem;
    SoundSystem soundSystem;
    CollisionSystem collisionSystem;
    LevelSystem levelSystem;
    entt::registry registry;
};
