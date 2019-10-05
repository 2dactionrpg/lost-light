// Header
#include "shield.hpp"

// internal
// #include "turtle.hpp"
// #include "fish.hpp"

#include "projectile.hpp"

// stlib
#include <algorithm>
#include <string>

#include <cmath>

Texture Shield::shield_texture;

bool Shield::init()
{
    // Load shared texture
    if (!shield_texture.is_valid()) {
        if (!shield_texture.load_from_file(textures_path("shield.png"))) {
            fprintf(stderr, "Failed to load projectile texture!");
            return false;
        }
    }

    // The position corresponds to the center of the texture
    float wr = shield_texture.width * 0.5f;
    float hr = shield_texture.height * 0.5f;

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
    // motion.position = { 0.f, 0.f };
    // motion.radians = 0.f;
    // motion.speed = 200.f;

    physics.scale = { 0.5f, 0.05f };

    // m_is_alive = true;
    // m_light_up_countdown_ms = -1.f;

    return true;
}

// Releases all graphics resources
void Shield::destroy()
{
    glDeleteBuffers(1, &mesh.vbo);
    glDeleteBuffers(1, &mesh.ibo);
    glDeleteBuffers(1, &mesh.vao);

    glDeleteShader(effect.vertex);
    glDeleteShader(effect.fragment);
    glDeleteShader(effect.program);
}

void Shield::increaseSize()
{
    physics.scale = { physics.scale.x * 2.f, physics.scale.y };
}

void Shield::draw(const mat3& projection)
{
    // Transformation code, see Rendering and Transformation in the template specification for more info
    // Incrementally updates transformation matrix, thus ORDER IS IMPORTANT
    transform.begin();
    transform.translate(motion.position);
    transform.rotate(motion.radians);
    transform.translate({ 0.f, 150.f });
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
    glBindTexture(GL_TEXTURE_2D, shield_texture.id);

    // Setting uniform values to the currently bound program
    glUniformMatrix3fv(transform_uloc, 1, GL_FALSE, (float*)&transform.out);
    float color[] = { 1.f, 1.f, 1.f };
    glUniform3fv(color_uloc, 1, color);
    glUniformMatrix3fv(projection_uloc, 1, GL_FALSE, (float*)&projection);

    // Drawing!
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, nullptr);
}

vec2 Shield::get_position() const
{
    return motion.position;
}

void Shield::set_position(vec2 position)
{
    motion.position = position;
}

void Shield::set_rotation(float radians)
{
    motion.radians = radians;
}

vec2 Shield::getDirection()
{
    vec2 direction = { sinf(motion.radians), -cosf(motion.radians) };
    float normal = sqrt(pow(direction.x, 2.f) + pow(direction.y, 2.f));
    direction = { direction.x / normal, direction.y / normal };
    return direction;
}

vec2 Shield::get_bounding_box() const
{
    // Returns the local bounding coordinates scaled by the current size of the projectile
    // fabs is to avoid negative scale due to the facing direction.
    return { std::fabs(physics.scale.x) * shield_texture.width, std::fabs(physics.scale.y) * shield_texture.height };
}

bool Shield::collides_with(const Projectile& projectile)
{
    vec3 tlMul = { -shield_texture.width / 2.f, -shield_texture.height / 2.f, 1.f };
    vec3 trMul = { +shield_texture.width / 2.f, -shield_texture.height / 2.f, 1.f };
    vec3 brMul = { +shield_texture.width / 2.f, +shield_texture.height / 2.f, 1.f };
    vec3 blMul = { -shield_texture.width / 2.f, +shield_texture.height / 2.f, 1.f };

    tlMul = mul(transform.out, tlMul);
    trMul = mul(transform.out, trMul);
    brMul = mul(transform.out, brMul);
    blMul = mul(transform.out, blMul);

    vec2 tl = { tlMul.x, tlMul.y };
    vec2 tr = { trMul.x, trMul.y };
    vec2 br = { brMul.x, brMul.y };
    vec2 bl = { blMul.x, blMul.y };

    float area1 = squareArea(tl, tr, bl);
    float area2 = trianglesArea(tl, tr, br, bl, projectile.get_position());
    if (std::fabs(area1 - area2) < 1000) {
        return true;
    }
    return false;
}

float Shield::triangleArea(vec2 p1, vec2 p2, vec2 p3)
{
    return 0.5f * std::fabs((p1.y * (p2.x - p3.x) + p2.y * (p3.x - p1.x) + p3.y * (p1.x - p2.x)));
}

float Shield::trianglesArea(vec2 p1, vec2 p2, vec2 p3, vec2 p4, vec2 projectileP)
{
    float area1 = triangleArea(p1, p2, projectileP);
    float area2 = triangleArea(p2, p3, projectileP);
    float area3 = triangleArea(p3, p4, projectileP);
    float area4 = triangleArea(p4, p1, projectileP);
    return area1 + area2 + area3 + area4;
}

float Shield::squareArea(vec2 p1, vec2 p2, vec2 p3)
{
    float side1 = sqrt(pow(p1.x - p2.x, 2) + pow(p1.y - p2.y, 2));
    float side2 = sqrt(pow(p1.x - p3.x, 2) + pow(p1.y - p3.y, 2));
    return side1 * side2;
}
