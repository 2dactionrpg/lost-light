#include "common.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "../ext/stb_image/stb_image.h"

// stlib
#include <cmath>
#include <iostream>
#include <sstream>
#include <vector>


void gl_flush_errors()
{
    while (glGetError() != GL_NO_ERROR)
        ;
}

bool gl_has_errors()
{
    GLenum error = glGetError();

    if (error == GL_NO_ERROR)
        return false;

    while (error != GL_NO_ERROR) {
        const char* error_str = "";
        switch (error) {
        case GL_INVALID_OPERATION:
            error_str = "INVALID_OPERATION";
            break;
        case GL_INVALID_ENUM:
            error_str = "INVALID_ENUM";
            break;
        case GL_INVALID_VALUE:
            error_str = "INVALID_VALUE";
            break;
        case GL_OUT_OF_MEMORY:
            error_str = "OUT_OF_MEMORY";
            break;
        case GL_INVALID_FRAMEBUFFER_OPERATION:
            error_str = "INVALID_FRAMEBUFFER_OPERATION";
            break;
        }

        fprintf(stderr, "OpenGL: %s", error_str);
        error = glGetError();
    }

    return true;
}

float dot(vec2 l, vec2 r)
{
    return l.x * r.x + l.y * r.y;
}

float dot(vec3 l, vec3 r)
{
    return l.x * r.x + l.y * r.y + l.z * r.z;
}

vec2 add(vec2 a, vec2 b) { return { a.x + b.x, a.y + b.y }; }
vec2 sub(vec2 a, vec2 b) { return { a.x - b.x, a.y - b.y }; }
vec2 mul(vec2 a, float b) { return { a.x * b, a.y * b }; }
vec3 mul(mat3 m, vec3 v) { return {
    dot(vec3 { m.c0.x, m.c1.x, m.c2.x }, v),
    dot(vec3 { m.c0.y, m.c1.y, m.c2.y }, v),
    dot(vec3 { m.c0.z, m.c1.z, m.c2.z }, v)
}; }
float sq_len(vec2 a) { return dot(a, a); }
float len(vec2 a) { return std::sqrt(sq_len(a)); }
vec2 to_vec2(vec3 v) { return { v.x, v.y }; }

mat3 mul(const mat3& l, const mat3& r)
{
    mat3 l_t = { { l.c0.x, l.c1.x, l.c2.x },
        { l.c0.y, l.c1.y, l.c2.y },
        { l.c0.z, l.c1.z, l.c2.z } };

    mat3 ret;
    ret.c0.x = dot(l_t.c0, r.c0);
    ret.c0.y = dot(l_t.c1, r.c0);
    ret.c0.z = dot(l_t.c2, r.c0);

    ret.c1.x = dot(l_t.c0, r.c1);
    ret.c1.y = dot(l_t.c1, r.c1);
    ret.c1.z = dot(l_t.c2, r.c1);

    ret.c2.x = dot(l_t.c0, r.c2);
    ret.c2.y = dot(l_t.c1, r.c2);
    ret.c2.z = dot(l_t.c2, r.c2);
    return ret;
}

vec2 normalize(vec2 v)
{
    if (!v.x && !v.y) {
        return v;
    }
    float m = sqrtf(dot(v, v));
    return { v.x / m, v.y / m };
}

Texture::Texture()
{
}

Texture::~Texture()
{
    if (id != 0)
        glDeleteTextures(1, &id);
    if (depth_render_buffer_id != 0)
        glDeleteRenderbuffers(1, &depth_render_buffer_id);
}

bool Texture::load_from_file(const char* path)
{
    if (path == nullptr)
        return false;

    stbi_uc* data = stbi_load(path, &width, &height, NULL, 4);
    if (data == NULL)
        return false;

    gl_flush_errors();
    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_2D, id);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    stbi_image_free(data);
    return !gl_has_errors();
}

// http://www.opengl-tutorial.org/intermediate-tutorials/tutorial-14-render-to-texture/
bool Texture::create_from_screen(GLFWwindow const* const window)
{
    gl_flush_errors();
    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_2D, id);

    glfwGetFramebufferSize(const_cast<GLFWwindow*>(window), &width, &height);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    // Generate the render buffer with the depth buffer
    glGenRenderbuffers(1, &depth_render_buffer_id);
    glBindRenderbuffer(GL_RENDERBUFFER, depth_render_buffer_id);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depth_render_buffer_id);

    // Set id as colour attachement #0
    glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, id, 0);

    // Set the list of draw buffers
    GLenum draw_buffers[1] = { GL_COLOR_ATTACHMENT0 };
    glDrawBuffers(1, draw_buffers); // "1" is the size of DrawBuffers

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        return false;

    return !gl_has_errors();
}

bool Texture::is_valid() const
{
    return id != 0;
}

namespace {
bool gl_compile_shader(GLuint shader)
{
    glCompileShader(shader);
    GLint success = 0;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (success == GL_FALSE) {
        GLint log_len;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &log_len);
        std::vector<char> log(log_len);
        glGetShaderInfoLog(shader, log_len, &log_len, log.data());
        glDeleteShader(shader);

        fprintf(stderr, "GLSL: %s", log.data());
        return false;
    }

    return true;
}
}

bool Entity::Effect::load_from_file(const char* vs_path, const char* fs_path)
{
    gl_flush_errors();

    // Opening files
    std::ifstream vs_is(vs_path);
    std::ifstream fs_is(fs_path);

    if (!vs_is.good() || !fs_is.good()) {
        fprintf(stderr, "Failed to load shader files %s, %s", vs_path, fs_path);
        return false;
    }

    // Reading sources
    std::stringstream vs_ss, fs_ss;
    vs_ss << vs_is.rdbuf();
    fs_ss << fs_is.rdbuf();
    std::string vs_str = vs_ss.str();
    std::string fs_str = fs_ss.str();
    const char* vs_src = vs_str.c_str();
    const char* fs_src = fs_str.c_str();
    GLsizei vs_len = (GLsizei)vs_str.size();
    GLsizei fs_len = (GLsizei)fs_str.size();

    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vs_src, &vs_len);
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fs_src, &fs_len);

    // Compiling
    // Shaders already delete if compilation fails
    if (!gl_compile_shader(vertex))
        return false;

    if (!gl_compile_shader(fragment)) {
        glDeleteShader(vertex);
        return false;
    }

    // Linking
    program = glCreateProgram();
    glAttachShader(program, vertex);
    glAttachShader(program, fragment);
    glLinkProgram(program);
    {
        GLint is_linked = 0;
        glGetProgramiv(program, GL_LINK_STATUS, &is_linked);
        if (is_linked == GL_FALSE) {
            GLint log_len;
            glGetProgramiv(program, GL_INFO_LOG_LENGTH, &log_len);
            std::vector<char> log(log_len);
            glGetProgramInfoLog(program, log_len, &log_len, log.data());

            release();
            fprintf(stderr, "Link error: %s", log.data());
            return false;
        }
    }

    if (gl_has_errors()) {
        release();
        fprintf(stderr, "OpenGL errors occured while compiling Effect");
        return false;
    }

    return true;
}

void Entity::Effect::release()
{
    glDeleteProgram(program);
    glDeleteShader(vertex);
    glDeleteShader(fragment);
}

void Entity::Transform::begin()
{
    out = { { 1.f, 0.f, 0.f }, { 0.f, 1.f, 0.f }, { 0.f, 0.f, 1.f } };
}

void Entity::Transform::scale(vec2 scale)
{
    mat3 S = { { scale.x, 0.f, 0.f }, { 0.f, scale.y, 0.f }, { 0.f, 0.f, 1.f } };
    out = mul(out, S);
}

void Entity::Transform::rotate(float radians)
{
    float c = cosf(radians);
    float s = sinf(radians);
    mat3 R = { { c, s, 0.f }, { -s, c, 0.f }, { 0.f, 0.f, 1.f } };
    out = mul(out, R);
}

void Entity::Transform::translate(vec2 offset)
{
    mat3 T = { { 1.f, 0.f, 0.f }, { 0.f, 1.f, 0.f }, { offset.x, offset.y, 1.f } };
    out = mul(out, T);
}

void Entity::Transform::end()
{
    //
}
