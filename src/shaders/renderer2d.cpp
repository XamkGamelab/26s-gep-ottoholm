#include "renderer2d.hpp"

#include <iterator>
#include <glad/gl.h>

namespace
{
	constexpr float quad_vertices[] = {
		//  X      Y
		-0.5f, -0.5f, // 0: bottom left
		 0.5f, -0.5f, // 1: bottom right
		 0.5f,  0.5f, // 2: top right
		-0.5f,  0.5f, // 3: top left
	};

	constexpr std::uint32_t quad_indices[] = {
		0, 1, 2,
		2, 3, 0,
	};

	constexpr GLsizei quad_index_count = static_cast<GLsizei>(std::size(quad_indices));
}

gep::graphics::renderer2d::~renderer2d()
{
	shutdown();
}

auto gep::graphics::renderer2d::init() -> bool
{
	// Get the shaders frag and vertr
	quad_shader.load("assets/shaders/vert.glsl", "assets/shaders/frag.glsl");

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	// Vertex data
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quad_vertices), quad_vertices, GL_STATIC_DRAW);

	// Index data
	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(quad_indices), quad_indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), nullptr);
	glEnableVertexAttribArray(0);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	return true;
}

auto gep::graphics::renderer2d::shutdown() noexcept -> void
{
	if (ebo != 0)
	{
		glDeleteBuffers(1, &ebo);
		ebo = 0;
	}

	if (vbo != 0)
	{
		glDeleteBuffers(1, &vbo);
		vbo = 0;
	}

	if (vao != 0)
	{
		glDeleteVertexArrays(1, &vao);
		vao = 0;
	}

	quad_shader.destroy();
}

auto gep::graphics::renderer2d::begin(std::uint8_t r, std::uint8_t g, std::uint8_t b) const noexcept -> void
{
	// OpenGL colors are 0-1 floats
 	glClearColor(r / 255.0f, g / 255.0f, b / 255.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
}

auto gep::graphics::renderer2d::set_viewport(int width, int height) const noexcept -> void
{
	glViewport(0, 0, width, height);
}

// Draw the quad square
auto gep::graphics::renderer2d::draw_quad() const noexcept -> void
{
	quad_shader.bind();
	glBindVertexArray(vao);

	glDrawElements(GL_TRIANGLES, quad_index_count, GL_UNSIGNED_INT, nullptr);

	glBindVertexArray(0);
	quad_shader.unbind();
}
