// Header
#include "enemy.hpp"

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
bool Enemy::init(int id)
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
    if (!effect.load_from_file(shader_path("enemy.vs.glsl"), shader_path("enemy.fs.glsl")))
        return false;

    // Setting initial values
    motion.position = { 0.f, 0.f };
    motion.radians = 4.75f;
    motion.speed = 200.f;

    enemy_id = id;

    physics.scale = { 0.4f, 0.4f };

    target = { 50.f, 300.f };

    m_is_alive = true;
    m_remain_dead_countdown_ms = -1.f;

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

vec2 Enemy::get_position() const
{
    return motion.position;
}

void Enemy::set_position(vec2 pos)
{
    motion.position = pos;
}

// void Enemy::move(vec2 off)
// {
//     motion.position.x += off.x;
//     motion.position.y += off.y;
//     if (motion.position.x > FRAME_X_MAX) {
//         motion.position.x = FRAME_X_MAX;
//     } else if (motion.position.x < FRAME_X_MIN) {
//         motion.position.x = FRAME_X_MIN;
//     }
//     if (motion.position.y > FRAME_Y_MAX) {
//         motion.position.y = FRAME_Y_MAX;
//     } else if (motion.position.y < FRAME_Y_MIN) {
//         motion.position.y = FRAME_Y_MIN;
//     }
// }

void Enemy::set_target(vec2 character_pos)
{
    if (target.x != character_pos.x || target.y != character_pos.y)
        target = character_pos;
}

void Enemy::set_rotation(float radians)
{
    motion.radians = radians;
}

void Enemy::set_scale(vec2 scale)
{
    physics.scale = scale;
}

bool Enemy::is_alive() const
{
    return m_is_alive;
}

void Enemy::kill()
{
    m_is_alive = false;
    m_remain_dead_countdown_ms = 2000.f;
}

void Enemy::respawn()
{
    m_is_alive = true;
    motion.position = { 0.f, 0.f };
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
    // float other_r = std::max(projectile.get_bounding_box().x, projectile.get_bounding_box().y);
    // float my_r = std::max(physics.scale.x, physics.scale.y);
    // float r = std::max(other_r, my_r);
    // r *= 0.6f;
    // if (d_sq < r * r)
    if (d_sq < 1000)
        return true;
    return false;
}

vec2 Enemy::get_face_position()
{
    vec3 tlMul = { 0, -enemy_texture.height / 2.f - 50, 1.f };
    tlMul = mul(transform.out, tlMul);
    return { tlMul.x, tlMul.y };
}

int Enemy::get_id()
{
    return enemy_id;
}
