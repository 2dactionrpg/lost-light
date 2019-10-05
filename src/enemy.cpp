// Header
#include "enemy.hpp"

// internal
// #include "turtle.hpp"
// #include "fish.hpp"

#include "projectile.hpp"

// stlib
#include <algorithm>
#include <string>

#include <cmath>

Texture Enemy::enemy_texture;

// following variables can change depending on the enemy's character size / world's frame size
float FRAME_X_MAX = 1050;
float FRAME_X_MIN = 100;
float FRAME_Y_MAX = 680;
float FRAME_Y_MIN = 100;
bool Enemy::init()
{
    // Load shared texture
    if (!enemy_texture.is_valid()) {
        if (!enemy_texture.load_from_file(textures_path("enemy.png"))) {
            fprintf(stderr, "Failed to load projectile texture!");
            return false;
        }
    }

    // The position corresponds to the center of the texture
    float wr = enemy_texture.width * 0.5f;
    float hr = enemy_texture.height * 0.5f;

    TexturedVertex vertices[4];
    vertices[0].position = { -wr, +hr, -0.02f };
    vertices[0].texcoord = { 0.f, 1.f };
    vertices[1].position = { +wr, +hr, -0.02f };
    vertices[1].texcoord = { 1.f, 1.f };
    vertices[2].position = { +wr, -hr, -0.02f };
    vertices[2].texcoord = { 1.f, 0.f };
    vertices[3].position = { -wr, -hr, -0.02f };
    vertices[3].texcoord = { 0.f, 0.f };

    // Counterclockwise as it's the default opengl front winding direction
    uint16_t indices[] = { 0, 3, 1, 1, 3, 2 };

    // Clearing errors
    gl_flush_errors();

    // Vertex Buffer creation
    glGenBuffers(1, &mesh.vbo);
    glBindBuffer(GL_ARRAY_BUFFER, mesh.vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(TexturedVertex) * 4, vertices, GL_STATIC_DRAW);

    // Index Buffer creation
    glGenBuffers(1, &mesh.ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint16_t) * 6, indices, GL_STATIC_DRAW);

    // Vertex Array (Container for Vertex + Index buffer)
    glGenVertexArrays(1, &mesh.vao);
    if (gl_has_errors())
        return false;

    // Loading shaders
    if (!effect.load_from_file(shader_path("textured.vs.glsl"), shader_path("textured.fs.glsl")))
        return false;

    // Setting initial values
    motion.position = { 1000.f, 300.f };
    motion.radians = 4.75f;
    motion.speed = 200.f;

    physics.scale = { -1.f, 1.f };

    direction = down;
    target = { 50.f, 300.f };

    m_is_alive = true;
    m_remain_dead_countdown_ms  = -1.f;

    return true;
}

// Releases all graphics resources
void Enemy::destroy()
{
    glDeleteBuffers(1, &mesh.vbo);
    glDeleteBuffers(1, &mesh.ibo);
    glDeleteBuffers(1, &mesh.vao);

    glDeleteShader(effect.vertex);
    glDeleteShader(effect.fragment);
    glDeleteShader(effect.program);
}

// Called on each frame by World::update()
void Enemy::update(float ms)
{
    float step = motion.speed * (ms / 1000);

    // Random movement
    srand(time(NULL));
    move({((rand()%3) - 1) * step, ((rand()%3) - 1) * step});
    
    if (m_remain_dead_countdown_ms  > 0.f)
        m_remain_dead_countdown_ms  -= ms;

    if (!m_is_alive && m_remain_dead_countdown_ms <= 0.f)
        respawn();

    // face the enemy to its target
    float rad = atan2(target.x - motion.position.x, motion.position.y - target.y);
    set_rotation(rad);
}

void Enemy::draw(const mat3& projection)
{
    // Transformation code, see Rendering and Transformation in the template specification for more info
    // Incrementally updates transformation matrix, thus ORDER IS IMPORTANT
    transform.begin();
    transform.translate(motion.position);
    transform.rotate(motion.radians);
    transform.scale(physics.scale);
    transform.end();

    // Setting shaders
    glUseProgram(effect.program);

    // Enabling alpha channel for textures
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDisable(GL_DEPTH_TEST);

    // Getting uniform locations for glUniform* calls
    GLint transform_uloc = glGetUniformLocation(effect.program, "transform");
    GLint color_uloc = glGetUniformLocation(effect.program, "fcolor");
    GLint projection_uloc = glGetUniformLocation(effect.program, "projection");

    // Setting vertices and indices
    glBindVertexArray(mesh.vao);
    glBindBuffer(GL_ARRAY_BUFFER, mesh.vbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.ibo);

    // Input data location as in the vertex buffer
    GLint in_position_loc = glGetAttribLocation(effect.program, "in_position");
    GLint in_texcoord_loc = glGetAttribLocation(effect.program, "in_texcoord");
    glEnableVertexAttribArray(in_position_loc);
    glEnableVertexAttribArray(in_texcoord_loc);
    glVertexAttribPointer(in_position_loc, 3, GL_FLOAT, GL_FALSE, sizeof(TexturedVertex), (void*)0);
    glVertexAttribPointer(in_texcoord_loc, 2, GL_FLOAT, GL_FALSE, sizeof(TexturedVertex), (void*)sizeof(vec3));

    // Enabling and binding texture to slot 0
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, enemy_texture.id);

    // Setting uniform values to the currently bound program
    glUniformMatrix3fv(transform_uloc, 1, GL_FALSE, (float*)&transform.out);
    float color[] = { 1.f, 1.f, 1.f };
    glUniform3fv(color_uloc, 1, color);
    glUniformMatrix3fv(projection_uloc, 1, GL_FALSE, (float*)&projection);

    // Drawing!
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, nullptr);
}

// Simple bounding box collision check
// This is a SUPER APPROXIMATE check that puts a circle around the bounding
// boxes and sees if the center point of either object is inside the other's
// bounding-box-circle. You don't need to try to use this technique. bool
// Character::collides_with(const Turtle& turtle)
// {
// 	float dx = motion.position.x - turtle.get_position().x;
// 	float dy = motion.position.y - turtle.get_position().y;
// 	float d_sq = dx * dx + dy * dy;
// 	float other_r = std::max(turtle.get_bounding_box().x,
// turtle.get_bounding_box().y); 	float my_r = std::max(physics.scale.x,
// physics.scale.y); 	float r = std::max(other_r, my_r); 	r *= 0.6f;
// if (d_sq < r
// * r) 		return true; 	return false;
// }

// bool Character::collides_with(const Fish& fish)
// {
// 	float dx = motion.position.x - fish.get_position().x;
// 	float dy = motion.position.y - fish.get_position().y;
// 	float d_sq = dx * dx + dy * dy;
// 	float other_r = std::max(fish.get_bounding_box().x,
// fish.get_bounding_box().y); 	float my_r = std::max(physics.scale.x,
// physics.scale.y); 	float r = std::max(other_r, my_r); 	r *= 0.6f;
// if (d_sq < r
// * r) 		return true; 	return false;
// }

// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
// HANDLE SALMON - WALL COLLISIONS HERE
// DON'T WORRY ABOUT THIS UNTIL ASSIGNMENT 2
// You will want to write new functions from scratch for checking/handling
// character - wall collisions.
// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

vec2 Enemy::get_position() const
{
    return motion.position;
}

void Enemy::move(vec2 off)
{
    motion.position.x += off.x;
    motion.position.y += off.y;
    if(motion.position.x > FRAME_X_MAX) {
        motion.position.x = FRAME_X_MAX;
    }else if(motion.position.x < FRAME_X_MIN) {
        motion.position.x = FRAME_X_MIN;
    }
    if(motion.position.y > FRAME_Y_MAX) {
        motion.position.y = FRAME_Y_MAX;
    }else if(motion.position.y < FRAME_Y_MIN) {
        motion.position.y = FRAME_Y_MIN;
    }
}

void Enemy::set_target(vec2 character_pos){
    if (target.x != character_pos.x || target.y != character_pos.y)
        target = character_pos;
}

void Enemy::set_rotation(float radians)
{
    motion.radians = radians;
}

bool Enemy::is_alive() const
{
    return m_is_alive;
}

// Called when the character collides with a turtle
void Enemy::kill()
{
    m_is_alive = false;
    m_remain_dead_countdown_ms = 2000.f;
}

void Enemy::respawn()
{
    m_is_alive = true;
    motion.position = { 1000.f, 0.f };
    direction = down;
}

Projectile Enemy::shoot_projectile() {
    Projectile projectlie;
    projectlie.setRotation(motion.radians+ 1.2);
    return projectlie;
}

vec2 Enemy::get_bounding_box() const
{
    // Returns the local bounding coordinates scaled by the current size of the projectile
    // fabs is to avoid negative scale due to the facing direction.
    return { std::fabs(physics.scale.x) * enemy_texture.width, std::fabs(physics.scale.y) * enemy_texture.height };
}

bool Enemy::collides_with(const Projectile& projectile)
{
    float dx = motion.position.x - projectile.get_position().x;
    float dy = motion.position.y - projectile.get_position().y;
    float d_sq = dx * dx + dy * dy;
    float other_r = std::max(projectile.get_bounding_box().x, projectile.get_bounding_box().y);
    float my_r = std::max(physics.scale.x, physics.scale.y);
    float r = std::max(other_r, my_r);
    r *= 0.6f;
    if (d_sq < r * r)
        return true;
    return false;
}