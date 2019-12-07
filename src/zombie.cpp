
#include "zombie.hpp"

#include <algorithm>
#include <string>

#include <cmath>

Texture Zombie::zombie_texture;

bool Zombie::init(int id)
{
    if (!zombie_texture.is_valid())
    {
        if (!zombie_texture.load_from_file(textures_path("zombie.png")))
        {
            fprintf(stderr, "Failed to load zombie texutre\n");
            return false;
        }
    }

    // the position corresponds to the center of the textures
    // fixed values
    float wr = zombie_texture.width * 0.5f;
    float hr = zombie_texture.height * 0.5f;

    TexturedVertex vertices[4];
    vertices[0].position = {-wr, +hr, -0.02f};
    vertices[0].texcoord = {0.f, 1.f};
    vertices[1].position = {+wr, +hr, -0.02f};
    vertices[1].texcoord = {1.f, 1.f};
    vertices[2].position = {+wr, -hr, -0.02f};
    vertices[2].texcoord = {1.f, 0.f};
    vertices[3].position = {-wr, -hr, -0.02f};
    vertices[3].texcoord = {0.f, 0.f};

    // Counterclockwise as it's the default opengl fron winding direction
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

    zombie_id = id;
    has_path = false;

    motion.position = {0.f, 0.f};
    motion.radians = 0.f;
    physics.scale = {0.f, 0.f};

    return true;
}

void Zombie::destroy()
{
    glDeleteBuffers(1, &mesh.vbo);
    glDeleteBuffers(1, &mesh.ibo);
    glDeleteVertexArrays(1, &mesh.vao);

    glDeleteShader(effect.vertex);
    glDeleteShader(effect.fragment);
    glDeleteShader(effect.program);
}

void Zombie::draw(const mat3 &projection)
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
    glBindTexture(GL_TEXTURE_2D, zombie_texture.id);

    // Setting uniform values to the currently bound program
    glUniformMatrix3fv(transform_uloc, 1, GL_FALSE, (float *)&transform.out);
    float color[] = {1.f, 1.f, 1.f};
    glUniform3fv(color_uloc, 1, color);
    glUniformMatrix3fv(projection_uloc, 1, GL_FALSE, (float *)&projection);

    // Drawing!
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, nullptr);
}

void Zombie::draw(const mat3 &projection, bool debug)
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
    glBindTexture(GL_TEXTURE_2D, zombie_texture.id);

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

vec2 Zombie::get_position() const
{
    return motion.position;
}

void Zombie::set_position(vec2 pos)
{
    motion.position = pos;
}

void Zombie::set_rotation(float radians)
{
    motion.radians = radians;
}

void Zombie::set_scale(vec2 scale)
{
    physics.scale = scale;
}

vec2 Zombie::get_bounding_box() const
{
    return {std::fabs(physics.scale.x) * zombie_texture.width - 50.f, std::fabs(physics.scale.y) * zombie_texture.height - 50.f};
}

bool Zombie::collides_with(const Projectile &projectile)
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

bool Zombie::collides_with_point(Dot d)
{
    vec2 box = get_bounding_box();
    box.x = box.x + 75.f;
    box.y = box.y + 75.f;
    float dx = motion.position.x - d.position.x;
    float dy = motion.position.y - d.position.y;
    float d_sq = dx * dx + dy * dy;
    float maxRadius = box.x / 2 + d.radius;
    if (d_sq < maxRadius * maxRadius)
        return true;
    return false;
}

vec2 Zombie::get_face_position()
{
    vec3 facePoint = {zombie_texture.width / 3.f, -zombie_texture.height / 2.f - 200, 1.f};
    facePoint = mul(transform.out, facePoint);
    return {facePoint.x, facePoint.y};
}

int Zombie::get_id()
{
    return zombie_id;
}

void Zombie::update_triangle()
{
    trig.set_position(get_face_position());
    trig.set_rotation(motion.radians + M_PI / 2);
    // trig.set_scale(physics.scale);
}
bool Zombie::on_sight(vec2 target)
{
    // fprintf(stderr, "in on_sight\n");
    // fprintf(stderr, "%d\n", trig.is_inside(target));
    return trig.is_inside(target);
}

// bool Zombie::set_line(vec2 target, std::vector<Enemy> &m_enemies)
// {
//     has_path = true;
//     std::vector<Dot> dots = path.form(get_face_position(), get_bounding_box(), target);
//     for (auto d : dots)
//     {
//         for (auto enemy : m_enemies)
//         {
//             if (enemy_id != enemy.get_id() && enemy.collides_with_point(d))
//             {
//                 path.blocked();
//                 return false;
//             }
//         }
//     }
//     return true;
// }

// void Zombie::unset_line()
// {
//     has_path = false;
// }

void Zombie::alert()
{
    trig.green();
}

void Zombie::search()
{
    trig.red();
}

void Zombie::idle()
{
    // fprintf(stderr, "in white\n");
    trig.white();
}