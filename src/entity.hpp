#pragma once
#include "common.hpp"

#include "../ext/stb_image/stb_image.h"

// stlib
#include <cmath>
#include <sstream>
#include <vector>

// Single Vertex Buffer element for non-textured meshes (coloured.vs.glsl & character.vs.glsl)
struct Vertex {
    vec3 position;
    vec3 color;
};

// Single Vertex Buffer element for textured sprites (textured.vs.glsl)
struct TexturedVertex {
    vec3 position;
    vec2 texcoord;
};

// Texture wrapper
struct Texture {
    Texture();
    ~Texture();

    GLuint id;
    GLuint depth_render_buffer_id;
    int width;
    int height;

    // Loads texture from file specified by path
    bool load_from_file(const char* path);
    bool is_valid() const; // True if texture is valid
    bool create_from_screen(GLFWwindow const* const window); // Screen texture
};

// An entity boils down to a collection of components,
// organized by their in-game context (mesh, effect, motion, etc...)
struct Entity {
    bool init();
    void destroy();
    // projection contains the orthographic projection matrix. As every Entity::draw()
    // renders itself it needs it to correctly bind it to its shader.
    virtual void draw(const mat3& projection) = 0;
    virtual void draw(const mat3& projection, bool debug=false) {};

protected:
    // A Mesh is a collection of a VertexBuffer and an IndexBuffer. A VAO
    // represents a Vertex Array Object and is the container for 1 or more Vertex Buffers and
    // an Index Buffer.
    struct Mesh {
        GLuint vao;
        GLuint vbo;
        GLuint ibo;
    } mesh;

    // Effect component of Entity for Vertex and Fragment shader, which are then put(linked) together in a
    // single program that is then bound to the pipeline.
    struct Effect {
        GLuint vertex;
        GLuint fragment;
        GLuint program;

        bool load_from_file(const char* vs_path, const char* fs_path); // load shaders from files and link into program
        void release(); // release shaders and program
    } effect;

    // All data relevant to the motion of the character.
    struct Motion {
        vec2 position;
        vec2 direction;
        float radians;
        float speed;
    } motion;

    // Scale is used in the bounding box calculations,
    // and so contextually belongs here (for now).
    struct Physics {
        vec2 scale;
        vec2 distortion;
        float sheer;
    } physics;

    // Transform component handles transformations passed to the Vertex shader.
    // gl Immediate mode equivalent, see the Rendering and Transformations section in the
    // specification pdf.
    struct Transform {
        mat3 out;

        void begin();
        void scale(vec2 scale);
        void rotate(float radians);
        void translate(vec2 offset);
        void sheer(float angle);
        void end();
    } transform;
};
