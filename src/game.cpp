#include "game.hpp"
#include <SDL3/SDL_init.h>
#include <SDL3/SDL_log.h>
#include <SDL3/SDL_video.h>
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_keycode.h>
#include <SDL3/SDL_render.h>

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
	renderer = SDL_CreateRenderer(handle, nullptr);
	if (renderer == nullptr)
	{
		SDL_LogError(SDL_LOG_CATEGORY_RENDER,
			"SDL_CreateRenderer: %s", SDL_GetError());
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
				is_running = false;
			}

			// Kaikki näppäimistön inputit
			if (event.type == SDL_EVENT_KEY_DOWN)
			{
				if (event.key.key == SDLK_ESCAPE)
				{
					is_running = false;
				}

				// Näytön värin vaihto WASD
				else if (event.key.key == SDLK_W)
				{
					SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // punanen
					SDL_RenderClear(renderer);
					SDL_RenderPresent(renderer);
				}
				else if (event.key.key == SDLK_A)
				{
					SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255); // vihreä
					SDL_RenderClear(renderer);
					SDL_RenderPresent(renderer);
				}
				else if (event.key.key == SDLK_S)
				{
					SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255); // sininen
					SDL_RenderClear(renderer);
					SDL_RenderPresent(renderer);
				}
				else if (event.key.key == SDLK_D)
				{
					SDL_SetRenderDrawColor(renderer, 0, 255, 255, 255); // keltainen
					SDL_RenderClear(renderer);
					SDL_RenderPresent(renderer);
				}
			}
		}
	}
}
auto gep::game::shutdown() noexcept -> void
{
	SDL_Quit();
}