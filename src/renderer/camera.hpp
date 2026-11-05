#pragma once
#include <cstdint>
#include <glm/glm.hpp>
#include "core/transform.hpp"

namespace gep::renderer
{
	class [[nodiscard]] camera
	{
	public:
		
		camera(int32_t viewport_width, int32_t viewport_height, float camera_size = 5.0f);

		// Getters
		auto get_projection_matrix() const noexcept -> const glm::mat4& { return projection_matrix; }
		auto get_view_matrix() const noexcept -> glm::mat4;
		auto get_position() const noexcept -> const glm::vec3& { return camera_transform.get_position(); }
		auto get_transform() noexcept -> core::transform& { return camera_transform; }
		auto get_transform() const noexcept -> const core::transform& { return camera_transform; }
		auto get_size() const noexcept -> float { return size; }

		// Setters
		auto set_position(const glm::vec3& new_position) noexcept -> void;
		auto translate(const glm::vec3& offset) noexcept -> void;
		auto set_viewport_size(int32_t viewport_width, int32_t viewport_height) noexcept -> void;
		auto set_size(float camera_size) noexcept -> void;

	private:
		core::transform camera_transform;


		int32_t width{ 1 }, height{ 1 };
		float size{ 5.0f };
		glm::mat4 projection_matrix{ 1.0f };

		auto resolve_projection_matrix(int32_t w, int32_t h, float s) noexcept -> void;
	};
}
