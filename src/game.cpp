#include "game.hpp"
#include <SDL3/SDL_init.h>
#include <SDL3/SDL_log.h>
#include <SDL3/SDL_video.h>
#include <SDL3/SDL_events.h>

auto gep::game::init() noexcept -> bool
{
	if(!SDL_Init(SDL_INIT_VIDEO))
	{
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, 
			"SDL_Init failed: %s", SDL_GetError());
		return false;
	}

	handle = SDL_CreateWindow("Gep", 1280, 720, SDL_WINDOW_RESIZABLE);
	if (handle == nullptr)
	{
		SDL_LogError(SDL_LOG_CATEGORY_VIDEO,
			"SDL_CreateWindow: %s", SDL_GetError());
		return false;
	}

	return true;
}
auto gep::game::run() -> void
{
	bool is_running = true;
	while (is_running)
	{
		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_EVENT_QUIT)
			{
				is_running == false;
			}
		}
	}
}
auto gep::game::shutdown() noexcept -> void
{
	SDL_Quit();
}