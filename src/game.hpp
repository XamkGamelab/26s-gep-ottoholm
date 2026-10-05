#pragma once
#include <SDL3/SDL_video.h>
#include <SDL3/SDL_render.h>
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
		SDL_Window* handle;
		SDL_Renderer* renderer;
		SDL_Texture* image;

		// Image position (top-left corner) and size, in pixels
		float image_x = 0.0f;
		float image_y = 0.0f;
		float image_w = 0.0f;
		float image_h = 0.0f;
	};
}