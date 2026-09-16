#pragma once
#include <SDL3/SDL_video.h>
#include <SDL3/SDL_render.h>

namespace gep
{
	class game
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
		SDL_Window* handle;
		SDL_Renderer* renderer;
	};
}