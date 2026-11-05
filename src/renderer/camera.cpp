#include "camera.hpp"
#include <glm/gtc/matrix_transform.hpp>

namespace gep::renderer
{
	camera::camera(int32_t viewport_width, int32_t viewport_height, float camera_size)
		: width(viewport_width), height(viewport_height), size(camera_size)
	{
		resolve_projection_matrix(width, height, size);
	}

	auto camera::get_view_matrix() const noexcept -> glm::mat4
	{
		return glm::inverse(camera_transform.get_model_matrix());
	}

	auto camera::set_position(const glm::vec3& new_position) noexcept -> void
	{
		camera_transform.set_position(new_position);
	}

	auto camera::translate(const glm::vec3& offset) noexcept -> void
	{
		camera_transform.translate(offset);
	}

	auto camera::set_viewport_size(int32_t viewport_width, int32_t viewport_height) noexcept -> void
	{
		width = viewport_width;
		height = viewport_height;
		resolve_projection_matrix(width, height, size);
	}

	auto camera::set_size(float camera_size) noexcept -> void
	{
		size = camera_size;
		resolve_projection_matrix(width, height, size);
	}

	auto camera::resolve_projection_matrix(int32_t w, int32_t h, float s) noexcept -> void
	{

		const float aspect_ratio = static_cast<float>(w) / static_cast<float>(h);
		const float world_height = s * 2.0f;
		const float world_width = world_height * aspect_ratio;

		
		const float left = -world_width * 0.5f;
		const float right = world_width * 0.5f;
		const float bottom = -world_height * 0.5f;
		const float top = world_height * 0.5f;

		
		projection_matrix = glm::ortho(left, right, bottom, top, -1.0f, 1.0f);
	}
}
