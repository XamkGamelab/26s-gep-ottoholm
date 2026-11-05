#include "transform.hpp"

namespace gep::core
{
	namespace
	{
		const glm::vec3 z_axis{ 0.0f, 0.0f, 1.0f };
	}

	transform::transform(const glm::vec3& start_position, float rotation_degrees, const glm::vec3& start_scale)
		: position(start_position), scale(start_scale)
	{
		set_rotation(rotation_degrees);
	}

	auto transform::get_model_matrix() const noexcept -> glm::mat4
	{
		// M = T * R * S --> the vertex is scaled first, then rotated, then moved to its position
		glm::mat4 model{ 1.0f };
		model = glm::translate(model, position);
		model = model * glm::mat4_cast(rotation);
		model = glm::scale(model, scale);
		return model;
	}

	auto transform::set_position(const glm::vec3& new_position) noexcept -> void
	{
		position = new_position;
	}

	auto transform::set_rotation(float degrees) noexcept -> void
	{
		rotation = glm::angleAxis(glm::radians(degrees), z_axis);
	}

	auto transform::set_scale(const glm::vec3& new_scale) noexcept -> void
	{
		scale = new_scale;
	}

	auto transform::translate(const glm::vec3& offset) noexcept -> void
	{
		position += offset;
	}

	auto transform::rotate(float degrees) noexcept -> void
	{
		// Add the new rotation on top of the old one, normalize to avoid drift over time
		rotation = glm::normalize(glm::angleAxis(glm::radians(degrees), z_axis) * rotation);
	}
}
