#pragma once
#include <string_view>
#include <cstdint>
#include <glm/fwd.hpp>

namespace gep::renderer
{
	class [[nodiscard]] shader
	{
	public:
		shader() = default;
		~shader();
		shader(std::string_view v_path, std::string_view f_path);

		auto get_id() const noexcept -> uint32_t { return id; }
		auto bind() const noexcept -> void;
		auto clear() -> void;

		auto set_int(const char* name, int32_t value) const noexcept -> void;
		auto set_mat4x4(const char* name, const glm::mat4x4& value) const noexcept -> void;

		shader(const shader&) = delete;
		shader& operator=(const shader&) = delete;

		shader(shader&& rhs) noexcept;
		shader& operator=(shader&& rhs) noexcept;

	private:
		uint32_t id{ 0 };

		auto compile_shader(uint32_t shader, const char* source) const -> bool;
		auto try_get_uniform_location(const char* name, int32_t& location) const noexcept -> bool;
	};
}