#pragma once
#include <chrono>

namespace gep::time
{
	class [[nodiscard]] time_manager
	{
	public:
		static auto instance() -> time_manager&
		{
			static time_manager static_instance;
			return static_instance;
		}

		time_manager(const time_manager&) = delete;
		time_manager(time_manager&&) = delete;
		time_manager& operator=(const time_manager&) = delete;
		time_manager& operator=(time_manager&&) = delete;

		auto tick() noexcept -> void;

		auto get_delta_time() const noexcept -> float;

	private:
		time_manager() = default;
		~time_manager() = default;

		std::chrono::steady_clock::time_point last_time;
		float delta_time = 0.0f;
		bool first_tick = true;
	};
}