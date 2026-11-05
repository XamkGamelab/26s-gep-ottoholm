#pragma once

struct SDL_Window;
struct SDL_GLContextState;

namespace gep
{
	class [[nodiscard]] game
	{
	public:
		/*  Vanha tapa alustaa
		bool init();
		void run();
		void shutdown();*/


		// Moderni c++
		[[nodiscard]]
		auto init() noexcept -> bool;
		auto run() -> void;
		auto shutdown() noexcept -> void;

	private:
		SDL_Window* window_handle;
		SDL_GLContextState* gl_context;
	};
}
