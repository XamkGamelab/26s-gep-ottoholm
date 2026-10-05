#include "game.hpp"
#include "input/input_system.hpp"

#include <SDL3/SDL_init.h>
#include <SDL3/SDL_log.h>
#include <SDL3/SDL_video.h>
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_keycode.h>
#include <SDL3/SDL_render.h>
#include <SDL3_image/SDL_image.h>


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

	
	image = IMG_LoadTexture(renderer, "assets/awesomeface.png");
	if (image == nullptr)
	{
		SDL_LogError(SDL_LOG_CATEGORY_RENDER,
			"IMG_LoadTexture failed: %s", SDL_GetError());
		return false;
	}
 
	SDL_GetTextureSize(image, &image_w, &image_h);
 
	// Get image stright to the middle of the screen
	int width, height;
	SDL_GetRenderOutputSize(renderer, &width, &height);
	image_x = (width - image_w) / 2.0f;
	image_y = (height - image_h) / 2.0f;

	return true;
}
auto gep::game::run() -> void
{
	bool is_running = true;
	auto& input_system = input::input_system::instance();

	// Define background color for changing them
	SDL_Color background{ 30, 30, 30, 255 };

	while (is_running)
	{
		input_system.update();

		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			input_system.process_input(event);

			if (event.type == SDL_EVENT_QUIT)
			{
				is_running = false;
			}

			if (event.type == SDL_EVENT_WINDOW_RESIZED)
			{
				int width;
				int height;
				SDL_GetWindowSize(handle, &width, &height);
				SDL_Log("Window resized to %d x %d", width, height);
			}
			if (event.type == SDL_EVENT_KEY_DOWN || event.type == SDL_EVENT_KEY_UP)
			{

				if (event.key.key == SDLK_W)
				{
					background = { 255, 0, 0, 255 }; // red
				}
				if (event.key.key == SDLK_A)
				{
					background = { 0, 255, 0, 255 }; // green
				}
				if (event.key.key == SDLK_S)
				{
					background = { 0, 0, 255, 255 }; // blue
				}
				if (event.key.key == SDLK_D)
				{
					background = { 255, 255, 0, 255 }; // yellow
				}
			}
		}

		int move_speed = 1;

		if (input_system.is_key_pressed(SDLK_W))
		{
			image_y -= move_speed;
		}
		if (input_system.is_key_pressed(SDLK_A))
		{
			image_x -= move_speed;
		}
		if (input_system.is_key_pressed(SDLK_S))
		{
			image_y += move_speed;
		}
		if (input_system.is_key_pressed(SDLK_D))
		{
			image_x += move_speed;
		}

		int win_width, win_height;
		SDL_GetWindowSize(handle, &win_width, &win_height);

		if (image_x < 0)
		{
			image_x = 0;
		}
		if (image_y < 0)
		{
			image_y = 0;
		}
		if (image_x > win_width - image->w)
		{
			image_x = win_width - image->w;
		}
		if (image_y > win_height - image->h)
		{
			image_y = win_height - image->h;
		}

		SDL_SetRenderDrawColor(renderer, background.r, background.g, background.b, background.a);
		SDL_RenderClear(renderer); // Clear the win

		const SDL_FRect dest{ image_x, image_y, image_w, image_h };
		SDL_RenderTexture(renderer, image, nullptr, &dest); // Draw the image and colors

		SDL_RenderPresent(renderer); // Show it
	}
}

auto gep::game::shutdown() noexcept -> void
{
	if (handle != nullptr)
	{
		SDL_DestroyWindow(handle);
	}

	SDL_Quit();
}