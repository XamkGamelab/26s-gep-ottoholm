#pragma once
#include <cstdint>
#include <string_view>

namespace gep::renderer
{
	class [[nodiscard]] texture
	{
	public:
		texture() = default;
		texture(std::string_view texture_path);
		~texture();

		auto get_id() const noexcept -> uint32_t { return id; }
		auto bind(uint16_t texture_slot_unit) const noexcept -> void;

		texture(const texture&) = delete;
		texture& operator=(const texture&) = delete;

		texture(texture&& rhs) noexcept;
		texture& operator=(texture&& rhs) noexcept;

	private:
		uint32_t id{ 0 };
		uint32_t width{ 0 }, height{ 0 };

		auto clear() noexcept -> void;
	};
}