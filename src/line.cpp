#include "line.hpp"

#include <iostream>

bool Line::init()
{
    m_vertices.clear();
    m_indicies.clear();

    constexpr float z = -0.1f;
    float x1 = 0.0f;
    float y1 = 0.0f;
    float x2 = 1.0f;
    float y2 = 1.0f;

    vec3 c = {1.0, 1.0, 1.0};

    m_vertices = {
        {{x1, y1, z}, c},
        {{x1, y2, z}, c},
        {{x2, y1, z}, c},
        {{x2, y2, z}, c} 
    };

    m_indicies = {
        {0 ,1},
        {1, 3},
        {3, 2},
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

    motion.position = { 50.0f, 40.0f };
    physics.scale = { 1100.f, 720.f };

    return true;
}

void Line::destroy()
{
    glDeleteBuffers(1, &mesh.vbo);
	glDeleteBuffers(1, &mesh.ibo);
	glDeleteBuffers(1, &mesh.vao);

	glDeleteShader(effect.vertex);
	glDeleteShader(effect.fragment);
	glDeleteShader(effect.program);
}

void Line::draw(const mat3& projection)
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

    float color[] = { 1.f, 0.f, 0.f };
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

std::vector<vec3> Line::get_vertices()
{
    transform.begin();
	transform.translate(motion.position);
	transform.rotate(motion.radians);
	transform.scale(physics.scale);
	transform.end();

    std::vector<vec3> vertices;
    for(auto& v : m_vertices)
	{
		vec3 pos = mul(transform.out, v.position);
		// fprintf(stderr, "%f %f %f\n", pos.x, pos.y, pos.z);
        vertices.emplace_back(pos);
	}

    return vertices;
}