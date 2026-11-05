#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/fwd.hpp>

namespace gep::core
{
	class [[nodiscard]] transform
	{
	public:
		transform() = default;
		transform(const glm::vec3& start_position, float rotation_degrees = 0.0f, const glm::vec3& start_scale = glm::vec3{ 1.0f });

		auto get_model_matrix() const noexcept -> glm::mat4;

		// getters
		auto get_position() const noexcept -> const glm::vec3& { return position; }
		auto get_rotation() const noexcept -> const glm::quat& { return rotation; }
		auto get_scale() const noexcept -> const glm::vec3& { return scale; }

		// setters
		auto set_position(const glm::vec3& new_position) noexcept -> void;
		auto set_rotation(float degrees) noexcept -> void; 
		auto set_scale(const glm::vec3& new_scale) noexcept -> void;

		
		auto translate(const glm::vec3& offset) noexcept -> void;
		auto rotate(float degrees) noexcept -> void; 

	private:
		glm::vec3 position{ 0.0f };
		glm::quat rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
		glm::vec3 scale{ 1.0f };
	};
}
