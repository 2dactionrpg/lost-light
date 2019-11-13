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

vec2 rotate_point(vec2 o, float rad, vec2 p)
{
    float s = sin(rad);
    float c = cos(rad);

    // translate point back to origin:
    p.x -= o.x;
    p.y -= o.y;

    // rotate point
    float xnew = p.x * c - p.y * s;
    float ynew = p.x * s + p.y * c;

    // translate point back:
    p.x = xnew + o.x;
    p.y = ynew + o.y;
    return p;
}

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

float randomFloat(float a, float b)
{
    float random = ((float)rand()) / (float)RAND_MAX;
    float diff = b - a;
    float r = random * diff;
    return a + r;
}