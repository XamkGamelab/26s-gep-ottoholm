#pragma once
#include <cstdint>
#include <string>

namespace gep::graphics
{
	class [[nodiscard]] shader
	{
	public:
		shader() = default;
		~shader();

		shader(const shader&) = delete;
		shader& operator=(const shader&) = delete;

		// Load the vertex and fragment paths
		[[nodiscard]]
		auto load(const std::string& vertex_path, const std::string& fragment_path) -> bool;

		auto bind() const noexcept -> void;
		auto unbind() const noexcept -> void;

		auto destroy() noexcept -> void;

	private:
		std::uint32_t program_id = 0;
	};
}
