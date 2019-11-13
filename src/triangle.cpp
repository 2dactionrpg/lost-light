#define _USE_MATH_DEFINES

#include "triangle.hpp"

#include <cmath>

constexpr float z = -0.1f;
vec3 c = {1.0, 1.0, 1.0};
// float in_color[] = {1.0, 1.0, 1.0};


bool Triangle::init(int id)
{
    m_vertices.clear();
    m_indicies.clear();


    // float x1 = 0.0f;
    // float y1 = 0.0f;
    // float x2 = 1.0f;
    // float y2 = 1.0f;

    vec2 p1 = {1.f, 1.f};

    m_vertices = {
        {{p1.x, p1.y, z}, c},
        {{p1.x - 550, p1.y - 250, z}, c},
        {{p1.x - 550, p1.y + 250, z}, c},
    };

    m_indicies = {
        {0 ,1},
        {1, 2},
        {2, 0}
    };

    // Clearing errors
    gl_flush_errors();

    // Vertex Buffer creation
    glGenBuffers(1, &mesh.vbo);
    glBindBuffer(GL_ARRAY_BUFFER, mesh.vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * m_vertices.size(), m_vertices.data(), GL_STATIC_DRAW);

	// Index Buffer creation
    glGenBuffers(1, &mesh.ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Line) * m_indicies.size(), m_indicies.data(), GL_STATIC_DRAW);

    glGenVertexArrays(1, &mesh.vao);

    if (gl_has_errors())
		return false;

    if (!effect.load_from_file(shader_path("coloured.vs.glsl"), shader_path("coloured.fs.glsl")))
		return false;

    trig_id = id;

    motion.position = { 50.0f, 40.0f };
    physics.scale = { 1.f, 1.f };

    in_color[0] = 1.0;
    in_color[1] = 1.0;
    in_color[2] = 1.0;

    return true;
}

void Triangle::destroy()
{
    glDeleteBuffers(1, &mesh.vbo);
    glDeleteBuffers(1, &mesh.ibo);
    glDeleteBuffers(1, &mesh.vao);

    glDeleteShader(effect.vertex);
    glDeleteShader(effect.fragment);
    glDeleteShader(effect.program);
}

void Triangle::draw(const mat3& projection)
{
    transform.begin();

    transform.translate(motion.position);
    transform.rotate(motion.radians);
    transform.scale(physics.scale);

    transform.end();

    glUseProgram(effect.program);

    glEnable(GL_BLEND); glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_DEPTH_TEST);
    GLint transform_uloc = glGetUniformLocation(effect.program, "transform");
    GLint color_uloc = glGetUniformLocation(effect.program, "color");
    GLint projection_uloc = glGetUniformLocation(effect.program, "projection");

    glBindVertexArray(mesh.vao);
    glBindBuffer(GL_ARRAY_BUFFER, mesh.vbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.ibo);

    GLint in_position_loc = glGetAttribLocation(effect.program, "in_position");
    glEnableVertexAttribArray(in_position_loc);
    glVertexAttribPointer(in_position_loc, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

    glUniformMatrix3fv(transform_uloc, 1, GL_FALSE, (float*)&transform.out);
    float color[] = {in_color[0], in_color[1], in_color[2]};
    glUniform3fv(color_uloc, 1, color);

    glUniformMatrix3fv(projection_uloc, 1, GL_FALSE, (float*)&projection);

    // GLint size = 0;
	// glBindBuffer(GL_ARRAY_BUFFER, mesh.vbo);
	// glGetBufferParameteriv(GL_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);
	// GLsizei num_indices = size / sizeof(uint16_t);

    GLint size = 0;
	glBindBuffer(GL_ARRAY_BUFFER, mesh.ibo);
	glGetBufferParameteriv(GL_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);
	GLsizei num_indices = size / sizeof(uint16_t);
    glDrawElements(GL_LINES, num_indices, GL_UNSIGNED_SHORT, nullptr);
}

void Triangle::set_position(vec2 pos)
{
    motion.position = pos;
}
void Triangle::set_rotation(float rad)
{
    motion.radians = rad;
}
void Triangle::set_scale(vec2 scale)
{
    physics.scale = scale;
}

void Triangle::green()
{
    in_color[0] = 0.0;
    in_color[1] = 1.0;
    in_color[2] = 0.0;
}
void Triangle::red()
{
    in_color[0] = 1.0;
    in_color[1] = 0.0;
    in_color[2] = 0.0;
}
void Triangle::white()
{
    in_color[0] = 1.0;
    in_color[1] = 1.0;
    in_color[2] = 1.0;
}

bool Triangle::is_inside(vec2 pos)
{
    vec2 p1 = motion.position;
    vec2 p2 = {p1.x - 550.f, p1.y - 250.f};
    p2 = rotate_point(p1, motion.radians, p2);
    vec2 p3 = {p1.x - 550.f, p1.y + 250.f};
    p3 = rotate_point(p1, motion.radians, p3);
    float A = area (p1, p2, p3); 

    /* Calculate area of triangle PBC */   
    float A1 = area (pos, p2, p3); 

    /* Calculate area of triangle PAC */   
    float A2 = area (p1, pos, p3); 

    /* Calculate area of triangle PAB */    
    float A3 = area (p1, p2, pos); 
    
    /* Check if sum of A1, A2 and A3 is same as A */
    // fprintf(stderr, "%f %f\n", A, A1 + A2 + A3);
    // fprintf(stderr, "%d\n", fabs(A - A1 - A2 - A3) < 0.04);

    // some marginal error (0.2) to cover floating points mistakes
    return (fabs(A - A1 - A2 - A3) < 0.04);
}

float Triangle::area(vec2 p1, vec2 p2, vec2 p3) 
{ 
    return fabs((p1.x*(p2.y-p3.y) + p2.x*(p3.y-p1.y)+ p3.x*(p1.y-p2.y))/2.f); 
} 