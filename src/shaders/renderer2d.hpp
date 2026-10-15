#pragma once
#include <cstdint>

#include "shaders/shader.hpp"

namespace gep::graphics
{
	class [[nodiscard]] renderer2d
	{
	public:
		renderer2d() = default;
		~renderer2d();

		renderer2d(const renderer2d&) = delete;
		renderer2d& operator=(const renderer2d&) = delete;

		[[nodiscard]]
		auto init() -> bool;

		// Empty
		auto shutdown() noexcept -> void;

		// Begin coloros
		auto begin(std::uint8_t r, std::uint8_t g, std::uint8_t b) const noexcept -> void;

		// Screen resizing
		auto set_viewport(int width, int height) const noexcept -> void;

		// Draf the quad
		auto draw_quad() const noexcept -> void;

	private:
		shader quad_shader;

		std::uint32_t vao = 0; // vertex array object
		std::uint32_t vbo = 0; // vertex buffer object
		std::uint32_t ebo = 0; // element buffer object
	};
}
