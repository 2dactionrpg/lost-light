// Header
#include "shield.hpp"

// stlib
#include <algorithm>
#include <string>

#include <cmath>

Texture Shield::shield_texture;

bool Shield::init()
{
    // Load shared texture
    if (!shield_texture.is_valid())
    {
        if (!shield_texture.load_from_file(textures_path("shield.png")))
        {
            fprintf(stderr, "Failed to load projectile texture!");
            return false;
        }
    }

    // The position corresponds to the center of the texture
    float wr = shield_texture.width * 0.5f;
    float hr = shield_texture.height * 0.5f;

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
    // This is where magic happens
    glGenBuffers(1, &mesh.vbo);
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
    if (!effect.load_from_file(shader_path("shield.vs.glsl"), shader_path("shield.fs.glsl")))
        return false;

    return true;
}

// Releases all graphics resources
void Shield::destroy()
{
    glDeleteBuffers(1, &mesh.vbo);
    glDeleteBuffers(1, &mesh.ibo);
    glDeleteVertexArrays(1, &mesh.vao);

    glDeleteShader(effect.vertex);
    glDeleteShader(effect.fragment);
    glDeleteShader(effect.program);
}

void Shield::draw(const mat3 &projection)
{
    // Transformation code, see Rendering and Transformation in the template specification for more info
    // Incrementally updates transformation matrix, thus ORDER IS IMPORTANT
    transform.begin();
    transform.translate(motion.position);
    transform.rotate(motion.radians);
    transform.translate({0.f, 80.f});
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
    glBindTexture(GL_TEXTURE_2D, shield_texture.id);

    // Setting uniform values to the currently bound program
    glUniformMatrix3fv(transform_uloc, 1, GL_FALSE, (float *)&transform.out);
    float color[] = {1.f, 1.f, 1.f};
    glUniform3fv(color_uloc, 1, color);
    glUniformMatrix3fv(projection_uloc, 1, GL_FALSE, (float *)&projection);

    // Drawing!
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, nullptr);
}

vec2 Shield::get_scale() const
{
    return physics.scale;
}

void Shield::set_scale(vec2 scale)
{
    physics.scale = scale;
}

void Shield::hide()
{
    physics.scale = {0.f, 0.f};
}

vec2 Shield::get_position() const
{
    return motion.position;
}

void Shield::set_position(vec2 position)
{
    motion.position = position;
}

float Shield::get_rotation() const
{
    return motion.radians;
}

void Shield::set_rotation(float radians)
{
    motion.radians = radians;
}

vec2 Shield::get_direction()
{
    vec2 direction = {sinf(motion.radians), -cosf(motion.radians)};
    float normal = sqrt(pow(direction.x, 2.f) + pow(direction.y, 2.f));
    direction = {direction.x / normal, direction.y / normal};
    return direction;
}

vec2 Shield::get_bounding_box() const
{
    // Returns the local bounding coordinates scaled by the current size of the projectile
    // fabs is to avoid negative scale due to the facing direction.
    return {std::fabs(physics.scale.x) * shield_texture.width, std::fabs(physics.scale.y) * shield_texture.height};
}

bool Shield::collides_with(const Projectile &projectile)
{
    vec3 brMul = {+shield_texture.width / 2.f, -shield_texture.height / 2.f, 1.f};
    vec3 blMul = {-shield_texture.width / 2.f, -shield_texture.height / 2.f, 1.f};

    brMul = mul(transform.out, brMul);
    blMul = mul(transform.out, blMul);

    vec2 shieldBR = {brMul.x, brMul.y};
    vec2 shieldBL = {blMul.x, blMul.y};

    vec2 position = projectile.get_position();
    vec2 projectileTL = {position.x - projectile.get_bounding_box().x / 2.0f,
                         position.y - projectile.get_bounding_box().y / 2.0f};
    vec2 projectileTR = {position.x + projectile.get_bounding_box().x / 2.0f,
                         position.y - projectile.get_bounding_box().y / 2.0f};
    vec2 projectileBL = {position.x - projectile.get_bounding_box().x / 2.0f,
                         position.y + projectile.get_bounding_box().y / 2.0f};
    vec2 projectileBR = {position.x + projectile.get_bounding_box().x / 2.0f,
                         position.y + projectile.get_bounding_box().y / 2.0f};

    return doIntersect(shieldBL, shieldBR, projectileBL, projectileBR) || doIntersect(shieldBL, shieldBR, projectileBR, projectileTR) || doIntersect(shieldBL, shieldBR, projectileTR, projectileTL) || doIntersect(shieldBL, shieldBR, projectileTL, projectileBL);
}

// Formulas from https://www.geeksforgeeks.org/check-if-two-given-line-segments-intersect/
bool Shield::onSegment(vec2 p, vec2 q, vec2 r)
{
    return q.x <= std::max(p.x, r.x) && q.x >= std::min(p.x, r.x) && q.y <= std::max(p.y, r.y) && q.y >= std::min(p.y, r.y);
}

int Shield::orientation(vec2 p, vec2 q, vec2 r)
{
    // From https://www.geeksforgeeks.org/orientation-3-ordered-points/
    int val = (q.y - p.y) * (r.x - q.x) - (q.x - p.x) * (r.y - q.y);

    if (val == 0)
    {
        return 0;
    }

    return (val > 0) ? 1 : 2;
}

bool Shield::doIntersect(vec2 p1, vec2 q1, vec2 p2, vec2 q2)
{
    // Find the four orientations needed for general and
    // special cases
    int o1 = orientation(p1, q1, p2);
    int o2 = orientation(p1, q1, q2);
    int o3 = orientation(p2, q2, p1);
    int o4 = orientation(p2, q2, q1);

    // General case
    if (o1 != o2 && o3 != o4)
        return true;

    // Special Cases
    // p1, q1 and p2 are colinear and p2 lies on segment p1q1
    if (o1 == 0 && onSegment(p1, p2, q1))
        return true;

    // p1, q1 and q2 are colinear and q2 lies on segment p1q1
    if (o2 == 0 && onSegment(p1, q2, q1))
        return true;

    // p2, q2 and p1 are colinear and p1 lies on segment p2q2
    if (o3 == 0 && onSegment(p2, p1, q2))
        return true;

    // p2, q2 and q1 are colinear and q1 lies on segment p2q2
    if (o4 == 0 && onSegment(p2, q1, q2))
        return true;

    return false; // Doesn't fall in any of the above cases
}
