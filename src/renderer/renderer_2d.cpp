#include "renderer_2d.hpp"
#include "texture.hpp"
#include "camera.hpp"
#include "core/transform.hpp"
#include <glad/gl.h>
#include <SDL3_image/SDL_image.h>
#include <SDL3/SDL_surface.h>
#include <SDL3/SDL_pixels.h>


namespace gep::renderer
{
	renderer_2d::renderer_2d()
	{
		shader_program = shader{ "assets/shaders/vert.glsl", "assets/shaders/frag.glsl" };
		init_quad();
	}

	renderer_2d::~renderer_2d()
	{
		glDeleteVertexArrays(1, &vao);
		glDeleteBuffers(1, &vbo);
		glDeleteBuffers(1, &ebo);
		shader_program.clear();
	}

	auto renderer_2d::clear() const noexcept -> void
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	auto renderer_2d::set_viewport(int32_t width, int32_t height) const noexcept -> void
	{
		glViewport(0, 0, width, height);
	}

	
	auto renderer_2d::draw_quad(const core::transform& transform, const texture& tex, const camera& camera) const -> void
	{
		shader_program.bind();
		tex.bind(0);

		shader_program.set_int("texture_0", 0);

		shader_program.set_mat4x4("u_projection", camera.get_projection_matrix());
		shader_program.set_mat4x4("u_view", camera.get_view_matrix());
		shader_program.set_mat4x4("u_model", transform.get_model_matrix());

		glBindVertexArray(vao);

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	}


	auto renderer_2d::init_quad() -> void
	{
		float vertices[] = {
			// X     Y     U     V
			 0.5f,  0.5f, 1.0f, 1.0f, // top right
			 0.5f, -0.5f, 1.0f, 0.0f,// bottom right
			-0.5f, -0.5f, 0.0f, 0.0f, // bottom left
			-0.5f,  0.5f, 0.0f, 1.0f // top left 
		};

		uint32_t indices[] = {  
			0, 1, 3,   
			1, 2, 3    
		};

		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);
		glGenBuffers(1, &vbo);
		glGenBuffers(1, &ebo);

		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

		uint32_t element_component_count = 2; // Vertex --> from 2 components
		uint32_t element_size = 4 * sizeof(float);
		uint32_t vertex_count = sizeof(vertices) / element_size;
		glVertexAttribPointer(0,
			element_component_count, GL_FLOAT, GL_FALSE, element_size, (void*)0);
		glEnableVertexAttribArray(0);

		glVertexAttribPointer(1,
			element_component_count, GL_FLOAT, GL_FALSE, element_size, (void*)(2 * sizeof(float)));
		glEnableVertexAttribArray(1);

	}
}