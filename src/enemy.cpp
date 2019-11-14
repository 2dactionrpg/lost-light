// Header
#include "enemy.hpp"

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
    if (!enemy_texture.is_valid())
    {
        if (!enemy_texture.load_from_file(textures_path("enemy.png")))
        {
            fprintf(stderr, "Failed to load projectile texture!");
            return false;
        }
    }

    // The position corresponds to the center of the texture
    float wr = enemy_texture.width * 0.5f;
    float hr = enemy_texture.height * 0.5f;

    TexturedVertex vertices[4];
    vertices[0].position = {-wr, +hr, -0.02f};
    vertices[0].texcoord = {0.f, 1.f};
    vertices[1].position = {+wr, +hr, -0.02f};
    vertices[1].texcoord = {1.f, 1.f};
    vertices[2].position = {+wr, -hr, -0.02f};
    vertices[2].texcoord = {1.f, 0.f};
    vertices[3].position = {-wr, -hr, -0.02f};
    vertices[3].texcoord = {0.f, 0.f};

    // Counterclockwise as it's the default opengl front winding direction
    uint16_t indices[] = {0, 3, 1, 1, 3, 2};

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

    enemy_id = id;
    has_path = false;

    motion.position = {0.f, 0.f};
    motion.radians = 0.f;
    physics.scale = {0.f, 0.f};

    // motion.radians = M_PI + M_PI / 2;

    return trig.init(id) && path.init(id);
}

// Releases all graphics resources
void Enemy::destroy()
{
    glDeleteBuffers(1, &mesh.vbo);
    glDeleteBuffers(1, &mesh.ibo);
    glDeleteVertexArrays(1, &mesh.vao);

    glDeleteShader(effect.vertex);
    glDeleteShader(effect.fragment);
    glDeleteShader(effect.program);
}

void Enemy::draw(const mat3 &projection)
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
    glVertexAttribPointer(in_position_loc, 3, GL_FLOAT, GL_FALSE, sizeof(TexturedVertex), (void *)0);
    glVertexAttribPointer(in_texcoord_loc, 2, GL_FLOAT, GL_FALSE, sizeof(TexturedVertex), (void *)sizeof(vec3));

    // Enabling and binding texture to slot 0
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, enemy_texture.id);

    // Setting uniform values to the currently bound program
    glUniformMatrix3fv(transform_uloc, 1, GL_FALSE, (float *)&transform.out);
    float color[] = {1.f, 1.f, 1.f};
    glUniform3fv(color_uloc, 1, color);
    glUniformMatrix3fv(projection_uloc, 1, GL_FALSE, (float *)&projection);

    // Drawing!
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, nullptr);
}

void Enemy::draw(const mat3 &projection, bool debug)
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
    glVertexAttribPointer(in_position_loc, 3, GL_FLOAT, GL_FALSE, sizeof(TexturedVertex), (void *)0);
    glVertexAttribPointer(in_texcoord_loc, 2, GL_FLOAT, GL_FALSE, sizeof(TexturedVertex), (void *)sizeof(vec3));

    // Enabling and binding texture to slot 0
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, enemy_texture.id);

    // Setting uniform values to the currently bound program
    glUniformMatrix3fv(transform_uloc, 1, GL_FALSE, (float *)&transform.out);
    float color[] = {1.f, 1.f, 1.f};
    glUniform3fv(color_uloc, 1, color);
    glUniformMatrix3fv(projection_uloc, 1, GL_FALSE, (float *)&projection);

    // Drawing!
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, nullptr);

    if (debug)
    {
        trig.draw(projection);
        if (has_path)
        {
            path.draw(projection);
        }
    }
}

vec2 Enemy::get_position() const
{
    return motion.position;
}

void Enemy::set_position(vec2 pos)
{
    motion.position = pos;
}

void Enemy::set_rotation(float radians)
{
    motion.radians = radians;
}

void Enemy::set_scale(vec2 scale)
{
    physics.scale = scale;
}

vec2 Enemy::get_bounding_box() const
{
    // Returns the local bounding coordinates scaled by the current size of the projectile
    // fabs is to avoid negative scale due to the facing direction.
    return {std::fabs(physics.scale.x) * enemy_texture.width, std::fabs(physics.scale.y) * enemy_texture.height};
}

bool Enemy::collides_with(const Projectile &projectile)
{
    vec2 box = get_bounding_box();
    float dx = motion.position.x - projectile.get_position().x;
    float dy = motion.position.y - projectile.get_position().y;
    float d_sq = dx * dx + dy * dy;
    float maxRadius = get_bounding_box().x / 2 + projectile.get_bounding_box().x / 2;
    if (d_sq < maxRadius * maxRadius)
        return true;
    return false;
}

bool Enemy::collides_with_point(Dot d)
{
    vec2 box = get_bounding_box();
    float dx = motion.position.x - d.position.x;
    float dy = motion.position.y - d.position.y;
    float d_sq = dx * dx + dy * dy;
    float maxRadius = get_bounding_box().x / 2 + d.radius;
    if (d_sq < maxRadius * maxRadius)
        return true;
    return false;
}

vec2 Enemy::get_face_position()
{
    vec3 facePoint = {enemy_texture.width / 3.f, -enemy_texture.height / 2.f - 200, 1.f};
    facePoint = mul(transform.out, facePoint);
    return {facePoint.x, facePoint.y};
}

int Enemy::get_id()
{
    return enemy_id;
}

void Enemy::update_triangle()
{
    trig.set_position(get_face_position());
    trig.set_rotation(motion.radians + M_PI / 2);
    // trig.set_scale(physics.scale);
}
bool Enemy::on_sight(vec2 target)
{
    // fprintf(stderr, "in on_sight\n");
    // fprintf(stderr, "%d\n", trig.is_inside(target));
    return trig.is_inside(target);
}

bool Enemy::set_line(vec2 target, std::vector<Enemy> &m_enemies)
{
    has_path = true;
    std::vector<Dot> dots = path.form(get_face_position(), get_bounding_box(), target);
    for (auto d : dots)
    {
        for (auto enemy : m_enemies)
        {
            if (enemy_id != enemy.get_id() && enemy.collides_with_point(d))
            {
                path.blocked();
                return false;
            }
        }
    }
    return true;
}
void Enemy::unset_line()
{
    has_path = false;
}

void Enemy::alert()
{
    trig.green();
}

void Enemy::search()
{
    trig.red();
}

void Enemy::idle()
{
    // fprintf(stderr, "in white\n");
    trig.white();
}