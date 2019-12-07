// Header
#include "overlay.hpp"

#include <cmath>

Texture Overlay::overlay_texture;

bool Overlay::init()
{
    // Load shared texture
    if (!overlay_texture.is_valid())
    {
        if (!overlay_texture.load_from_file(textures_path("overlay.png")))
        {
            fprintf(stderr, "Failed to load overlay texture!");
            return false;
        }
    }

    // The position corresponds to the center of the texture
    float wr = overlay_texture.width * 0.5f;
    float hr = overlay_texture.height * 0.5f;

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
    if (!effect.load_from_file(shader_path("overlay.vs.glsl"), shader_path("overlay.fs.glsl")))
        return false;

    motion.position = { 600.f, 400.f };
    physics.scale = { 1.f, 1.f };
    light_source = {0.25, 0.5};

    return true;
}

// Releases all graphics resources
void Overlay::destroy()
{
    glDeleteBuffers(1, &mesh.vbo);
    glDeleteBuffers(1, &mesh.ibo);
    glDeleteVertexArrays(1, &mesh.vao);

    glDeleteShader(effect.vertex);
    glDeleteShader(effect.fragment);
    glDeleteShader(effect.program);
}

void Overlay::load_texture(int state)
{
    switch (state)
    {
    case STATE_START:
        overlay_texture.load_from_file(textures_path("start.png"));
        break;
    case STATE_PLAYING:
        overlay_texture.load_from_file(textures_path("playing.png"));
        break;
    case STATE_PAUSE:
        overlay_texture.load_from_file(textures_path("pause.png"));
        break;
    case STATE_GAMEOVER:
        overlay_texture.load_from_file(textures_path("game-over.png"));
        break;
    case STATE_WIN:
        overlay_texture.load_from_file(textures_path("win.png"));
        break;
    case STATE_TUTORIAL:
        overlay_texture.load_from_file(textures_path("tutorial.png"));
        break;
    default:
        break;
    }
}

vec2 Overlay::get_direction()
{
    return motion.direction;
}

void Overlay::set_direction(vec2 direction)
{
    float normal = sqrt(pow(direction.x, 2.f) + pow(direction.y, 2.f));
    motion.direction = {direction.x / normal, direction.y / normal};
}

void Overlay::set_rotation(float rad)
{
    motion.radians = rad;
}

void Overlay::draw(const mat3 &projection)
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
    GLint light_source_x_uloc = glGetUniformLocation(effect.program, "light_source_x");
    GLint light_source_y_uloc = glGetUniformLocation(effect.program, "light_source_y");
    GLint radius_uloc = glGetUniformLocation(effect.program, "radius");
    GLint ls_num_uloc = glGetUniformLocation(effect.program, "ls_num");

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
    glBindTexture(GL_TEXTURE_2D, overlay_texture.id);

    // Setting uniform values to the currently bound program
    glUniformMatrix3fv(transform_uloc, 1, GL_FALSE, (float *)&transform.out);
    float color[] = {1.f, 1.f, 1.f};
    glUniform3fv(color_uloc, 1, color);
    glUniformMatrix3fv(projection_uloc, 1, GL_FALSE, (float *)&projection);
    GLfloat xs[10] = {light_source.x, 0.75, 0.5};
    GLfloat ys[10] = {light_source.y, 0.5, 0.2};
    GLfloat radius[10] = {0.5, 0.15, 0.15};

    int size = sizeof(xs)/sizeof(xs[0]);

    glUniform1fv(light_source_x_uloc, 10, xs);
    glUniform1fv(light_source_y_uloc, 10, ys);
    glUniform1fv(radius_uloc, 10, radius);
    glUniform1i(ls_num_uloc, size);

    // Drawing!
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, nullptr);
}

void Overlay::set_light_source(vec2 position) {
    const float screen_width = 1200.f;
    const float screen_height = 800.f;
    vec2 ls = {position.x/screen_width, position.y/screen_height};
    light_source = ls;
}


vec2 Overlay::get_position() const
{
    return motion.position;
}

void Overlay::set_position(vec2 position)
{
    motion.position = position;
}

vec2 Overlay::get_bounding_box() const
{
    // Returns the local bounding coordinates scaled by the current size of the overlay
    // fabs is to avoid negative scale due to the facing direction.
    return {std::fabs(physics.scale.x) * overlay_texture.width * 2,
            std::fabs(physics.scale.y) * overlay_texture.height * 2};
}
