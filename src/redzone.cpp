// Header
#include "redzone.hpp"

#include <vector>

Texture Redzone::redzone_texture;

bool Redzone::init()
{

    // Load shared texture
    if (!redzone_texture.is_valid())
    {
        if (!redzone_texture.load_from_file(textures_path("lava.png")))
        {
            fprintf(stderr, "Failed to load lava texture!");
            return false;
        }
    }

    // The position corresponds to the center of the texture
    float wr = redzone_texture.width * 0.5f;
    float hr = redzone_texture.height * 0.5f;

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
    if (!effect.load_from_file(shader_path("redzone.vs.glsl"), shader_path("redzone.fs.glsl")))
        return false;
    physics.scale = {0.1f, 0.1f};
    motion.position = {-100.f, -100.f};
    cooldown = 0.f;
    duration = 0.f;

    return true;
}

// Releases all graphics resources
void Redzone::destroy()
{
    glDeleteBuffers(1, &mesh.vbo);
    glDeleteBuffers(1, &mesh.ibo);
    glDeleteBuffers(1, &mesh.vao);

    glDeleteShader(effect.vertex);
    glDeleteShader(effect.fragment);
    glDeleteShader(effect.program);
}

void Redzone::draw(const mat3 &projection)
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
    GLint cooldown_uloc = glGetUniformLocation(effect.program, "cooldown");

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
    glBindTexture(GL_TEXTURE_2D, redzone_texture.id);

    // Setting uniform values to the currently bound program
    glUniformMatrix3fv(transform_uloc, 1, GL_FALSE, (float *)&transform.out);
    float color[] = {1.f, 1.f, 1.f};
    glUniform3fv(color_uloc, 1, color);
    glUniformMatrix3fv(projection_uloc, 1, GL_FALSE, (float *)&projection);
    glUniform1f(cooldown_uloc, cooldown);

    // Drawing!
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, nullptr);
}

void Redzone::set_position(vec2 pos)
{
    motion.position = pos;
}

vec2 Redzone::get_bounding_box() const
{
    return {std::fabs(physics.scale.x) * redzone_texture.width * 2,
            std::fabs(physics.scale.y) * redzone_texture.height * 2};
}

vec2 Redzone::get_position() const
{
    return motion.position;
}

void Redzone::set_cooldown(float m_cooldown)
{
    cooldown = m_cooldown;
}

void Redzone::set_duration(float m_duration)
{
    duration = m_duration;
}
