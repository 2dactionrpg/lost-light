#include "entity.hpp"

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

bool Texture::load_from_file(const char *path)
{
    if (path == nullptr)
        return false;

    stbi_uc *data = stbi_load(path, &width, &height, NULL, 4);
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
bool Texture::create_from_screen(GLFWwindow const *const window)
{
    gl_flush_errors();
    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_2D, id);

    glfwGetFramebufferSize(const_cast<GLFWwindow *>(window), &width, &height);

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
    GLenum draw_buffers[1] = {GL_COLOR_ATTACHMENT0};
    glDrawBuffers(1, draw_buffers); // "1" is the size of DrawBuffers

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        return false;

    return !gl_has_errors();
}

bool Texture::is_valid() const
{
    return id != 0;
}

namespace
{
bool gl_compile_shader(GLuint shader)
{
    glCompileShader(shader);
    GLint success = 0;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (success == GL_FALSE)
    {
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
} // namespace

bool Entity::Effect::load_from_file(const char *vs_path, const char *fs_path)
{
    gl_flush_errors();

    // Opening files
    std::ifstream vs_is(vs_path);
    std::ifstream fs_is(fs_path);

    if (!vs_is.good() || !fs_is.good())
    {
        fprintf(stderr, "Failed to load shader files %s, %s", vs_path, fs_path);
        return false;
    }

    // Reading sources
    std::stringstream vs_ss, fs_ss;
    vs_ss << vs_is.rdbuf();
    fs_ss << fs_is.rdbuf();
    std::string vs_str = vs_ss.str();
    std::string fs_str = fs_ss.str();
    const char *vs_src = vs_str.c_str();
    const char *fs_src = fs_str.c_str();
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

    if (!gl_compile_shader(fragment))
    {
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
        if (is_linked == GL_FALSE)
        {
            GLint log_len;
            glGetProgramiv(program, GL_INFO_LOG_LENGTH, &log_len);
            std::vector<char> log(log_len);
            glGetProgramInfoLog(program, log_len, &log_len, log.data());

            release();
            fprintf(stderr, "Link error: %s", log.data());
            return false;
        }
    }

    if (gl_has_errors())
    {
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
    out = {{1.f, 0.f, 0.f}, {0.f, 1.f, 0.f}, {0.f, 0.f, 1.f}};
}

void Entity::Transform::scale(vec2 scale)
{
    mat3 S = {{scale.x, 0.f, 0.f}, {0.f, scale.y, 0.f}, {0.f, 0.f, 1.f}};
    out = mul(out, S);
}

void Entity::Transform::rotate(float radians)
{
    float c = cosf(radians);
    float s = sinf(radians);
    mat3 R = {{c, s, 0.f}, {-s, c, 0.f}, {0.f, 0.f, 1.f}};
    out = mul(out, R);
}

void Entity::Transform::translate(vec2 offset)
{
    mat3 T = {{1.f, 0.f, 0.f}, {0.f, 1.f, 0.f}, {offset.x, offset.y, 1.f}};
    out = mul(out, T);
}

void Entity::Transform::sheer(float angle)
{
    mat3 T = {{1.f, tanf(angle), 0.f}, {tanf(angle), 1.f, 0.f}, {0.f, 0.f, 1.f}};
    out = mul(out, T);
}


void Entity::Transform::end()
{
}
