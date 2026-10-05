#include "time_manager.hpp"
#include <algorithm>

auto gep::time::time_manager::tick() noexcept -> void
{
	auto now = std::chrono::steady_clock::now();

	if (first_tick) 
	{
		delta_time = 0.0f;
		first_tick = false;
	}
	else 
	{
		delta_time = std::chrono::duration<float>(now - last_time).count();
		delta_time = std::min(delta_time, 1.0f / 30.0f);
	}

	last_time = now;
}

auto gep::time::time_manager::get_delta_time() const noexcept -> float
{
	return delta_time;
}