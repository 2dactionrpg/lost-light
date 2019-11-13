#pragma once

// stlib
#include <fstream> // stdout, stderr..

// glfw
#define NOMINMAX
#include <gl3w.h>

#include <GLFW/glfw3.h>

// Simple utility macros to avoid mistyping directory name, name has to be a string literal
// audio_path("audio.ogg") -> data/audio/audio.ogg
// Get defintion of PROJECT_SOURCE_DIR from:
#include "project_path.hpp"

#define shader_path(name) PROJECT_SOURCE_DIR "./shaders/" name

#define data_path PROJECT_SOURCE_DIR "./data"
#define textures_path(name) data_path "/textures/" name
#define audio_path(name) data_path "/audio/" name
#define mesh_path(name) data_path "/meshes/" name

// Not much math is needed and there are already way too many libraries linked (:
// If you want to do some overloads..
struct vec2 {
    float x, y;
};
struct vec3 {
    float x, y, z;
};
struct mat3 {
    vec3 c0, c1, c2;
};

// Utility functions
float dot(vec2 l, vec2 r);
float dot(vec3 l, vec3 r);
mat3 mul(const mat3& l, const mat3& r);
vec2 mul(vec2 a, float b);
vec3 mul(mat3 m, vec3 v);
vec2 normalize(vec2 v);
vec2 add(vec2 a, vec2 b);
vec2 sub(vec2 a, vec2 b);
vec2 to_vec2(vec3 v);
float sq_len(vec2 a);
float len(vec2 a);
vec2 rotate_point(vec2 o, float rad, vec2 p);

// OpenGL utilities
// cleans error buffer
void gl_flush_errors();
bool gl_has_errors();

float randomFloat(float a, float b);