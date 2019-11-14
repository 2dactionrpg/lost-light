// Header
#include "wall.hpp"
#include <iostream>

#include <cmath>

Texture Wall::wall_texture;

bool Wall::init(float xPos, float yPos)
{
    if (!wall_texture.is_valid()) {
        if (!wall_texture.load_from_file(textures_path("wall.png"))) {
            fprintf(stderr, "Failed to load wall texture!");
            return false;
        }
    }

    // The position corresponds to the center of the texture
    float wr;
    float hr;
    wr = wall_texture.width * 0.5f;
    hr = wall_texture.height * 0.5f;
    
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
    if (!effect.load_from_file(shader_path("wall.vs.glsl"), shader_path("wall.fs.glsl")))
        return false;

    //fprintf(stderr, "%f\t%f\n", xPos, yPos);

    motion.position = { xPos, yPos};
    motion.radians = 0;
    //setDirection({ -1.f, 0.2f });

    // Setting initial values, scale is negative to make it face the opposite way
    // 1.0 would be as big as the original texture.
    physics.scale = { -0.1f, 0.1f };
    m_is_alive = true;

    return true;
}

// Releases all graphics resources
void Wall::destroy()
{
    m_is_alive = false;
    glDeleteBuffers(1, &mesh.vbo);
    glDeleteBuffers(1, &mesh.ibo);
    glDeleteBuffers(1, &mesh.vao);

    glDeleteShader(effect.vertex);
    glDeleteShader(effect.fragment);
    glDeleteShader(effect.program);
}

void Wall::update(float ms)
{
}

vec2 Wall::getDirection()
{
    return motion.direction;
}

void Wall::setDirection(vec2 direction)
{
    float normal = sqrt(pow(direction.x, 2.f) + pow(direction.y, 2.f));
    motion.direction = { direction.x / normal, direction.y / normal };
}

void Wall::setRotation(float rad)
{
    motion.radians = rad;
}

void Wall::draw(const mat3& projection)
{
    //std::cout << "drawn" << std::endl;
    //fprintf(stderr, "%f\t%f\n", motion.position.x, motion.position.y);
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
    glBindTexture(GL_TEXTURE_2D, wall_texture.id);

    // Setting uniform values to the currently bound program
    glUniformMatrix3fv(transform_uloc, 1, GL_FALSE, (float*)&transform.out);
    float color[] = { 1.f, 1.f, 1.f };
    glUniform3fv(color_uloc, 1, color);
    glUniformMatrix3fv(projection_uloc, 1, GL_FALSE, (float*)&projection);

    // Drawing!
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, nullptr);
}

vec2 Wall::get_position() const
{
    return motion.position;
}

void Wall::set_position(vec2 position)
{
    motion.position = position;
}

vec2 Wall::get_bounding_box() const
{
    // Returns the local bounding coordinates scaled by the current size of the wall
    // fabs is to avoid negative scale due to the facing direction.
    return { std::fabs(physics.scale.x) * wall_texture.width * 2,
        std::fabs(physics.scale.y) * wall_texture.height * 2 };
}

bool Wall::is_alive() const
{
    return m_is_alive;
}

bool Wall::collides_with(const Projectile& projectile)
{
    vec2 box = get_bounding_box();
    float dx = motion.position.x - projectile.get_position().x;
    float dy = motion.position.y - projectile.get_position().y;
    float d_sq = dx * dx + dy * dy;
    float maxRadius = get_bounding_box().x / 3 + projectile.get_bounding_box().x / 3;
    if (d_sq < maxRadius * maxRadius)
        return true;
    return false;
}

void Wall::wall_offset(bool isBoss, vec2 position, vec2 offset, bool &right_moveable, bool &left_moveable, bool &up_moveable, bool &down_moveable)
{
    float grace = 60.f;
    if (isBoss) {
        grace *=2;
    }
    float objPosX = position.x;
    float objPosY = position.y;
    float finalObjPosx = objPosX + offset.x;
    float finalObjPosy = objPosY + offset.y;
    float wallPosLeftX = motion.position.x-grace;
    float wallPosRightX = wallPosLeftX + wall_texture.width * -physics.scale.x + grace;
    float wallPosTopY = motion.position.y - grace;
    float wallPosBottomY = wallPosTopY + wall_texture.height * physics.scale.y +grace;
    float wallMiddleX = wallPosLeftX  +  (wall_texture.width * -physics.scale.x) / 2;
    float wallMiddleY  = wallPosTopY + (wall_texture.height * physics.scale.y) / 2;

    if (right_moveable) {
        if (objPosX < wallMiddleX && objPosY > wallPosTopY && objPosY < wallPosBottomY && finalObjPosx > wallPosLeftX)  {
            right_moveable = false;
        }
    }

    if (left_moveable) {
        if (objPosX > wallMiddleX && objPosY > wallPosTopY && objPosY < wallPosBottomY && finalObjPosx < wallPosRightX) {
            left_moveable = false;
        }
    }

    if (up_moveable) {
        if(objPosY > wallMiddleY && objPosX < wallPosRightX && objPosX > wallPosLeftX && finalObjPosy < wallPosBottomY) {
            up_moveable = false;
        }
    }

    if (down_moveable) {
        if (objPosY < wallMiddleY && objPosX < wallPosRightX && objPosX > wallPosLeftX && finalObjPosy > wallPosTopY) {
            down_moveable = false;
        }
    }
}