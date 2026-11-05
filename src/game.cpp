#include <glad/gl.h>
#include "game.hpp"

#include <cmath>
#include <string>
#include <SDL3/SDL_init.h>
#include <SDL3/SDL_log.h>
#include <SDL3/SDL_video.h>
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_mouse.h>

#include <SDL3_image/SDL_image.h>

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

#include "core/transform.hpp"
#include "input/input_system.hpp"
#include "time/time_manager.hpp"
#include "renderer/camera.hpp"
#include "renderer/renderer_2d.hpp"
#include "renderer/texture.hpp"

auto gep::game::init() noexcept -> bool
{
	if(!SDL_Init(SDL_INIT_VIDEO))
	{
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, 
			"SDL_Init failed: %s", SDL_GetError());
		return false;
	}

	// configure SDL to request OpenGL 4.6
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

	// Kaikkien jutuskojen tarkistus, että ei oo tyhjiä, muuten erroria naamariin
	window_handle = SDL_CreateWindow("Gep", 1280, 720, SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL);
	if (window_handle == nullptr)
	{
		SDL_LogError(SDL_LOG_CATEGORY_VIDEO,
			"SDL_CreateWindow: %s", SDL_GetError());
		return false;
	}

	gl_context = SDL_GL_CreateContext(window_handle);
	auto version = gladLoadGL((GLADloadfunc)SDL_GL_GetProcAddress);
	if (version == 0)
	{
		SDL_LogError(SDL_LOG_CATEGORY_SYSTEM,
			"SDL_GL_CreateContext failed: %s", SDL_GetError());
		return false;
	}

	int32_t major, minor, profile;
	SDL_GL_GetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, &major);
	SDL_GL_GetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, &minor);
	SDL_GL_GetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, &profile);

	SDL_Log("OpenGL version: %i.%i - Profile: %i", major, minor, profile);
	SDL_Log("GLAD version: %i.%i", GLAD_VERSION_MAJOR(version), GLAD_VERSION_MINOR(version));

	glEnable(GL_BLEND);
	glClearColor(58 / 255.f, 51 / 255.f, 51 / 255.f, 1.0f);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	return true;
}
auto gep::game::run() -> void
{
	int32_t window_width;
	int32_t window_height;
	SDL_GetWindowSize(window_handle, &window_width, &window_height);

	auto& input_system = input::input_system::instance();
	auto& time_manager = time::time_manager::instance();

	renderer::renderer_2d renderer{};
	renderer::camera camera{ window_width, window_height, 5.0f };
	renderer::texture tex{ "assets/awesomeface.png" };
	auto speed = 5.0f; 

	// i=postition change, 2=rotation change, 3=scale change
	core::transform transform0{ glm::vec3{ -5.0f, 0.0f, 0.0f }, 0.0f, glm::vec3{ 2.0f } };  // moves up and down
	core::transform transform1{ glm::vec3{ 0.0f, 0.0f, 0.0f }, 0.0f, glm::vec3{ 3.0f } };   // rotates
	core::transform transform2{ glm::vec3{ 5.0f, 0.0f, 0.0f }, 15.0f, glm::vec3{ 2.0f } };  // pulses in size

	float elapsed_time = 0.0f;

	bool is_running = true;
	while (is_running)
	{
		time_manager.tick();
		const float delta_time = time_manager.get_delta_time();
		elapsed_time += delta_time;

		input_system.update();

		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_EVENT_QUIT)
			{
				is_running = false;
				return;
			}

			if (event.type == SDL_EVENT_WINDOW_RESIZED)
			{
				SDL_GetWindowSize(window_handle, &window_width, &window_height);
				SDL_Log("Window resized: %i %i", window_width, window_height);
			}

			// Keeps camera and viewport in sync with screen size. So it wont go over the screen
			renderer.set_viewport(window_width, window_height);
			camera.set_viewport_size(window_width, window_height);

			input_system.process_input(event);
		}
		
		// wasd
		glm::vec3 camera_direction{ 0.0f };
		if (input_system.is_key_pressed(SDLK_W))
		{
			camera_direction.y += 1.0f;
		}
		if (input_system.is_key_pressed(SDLK_S))
		{
			camera_direction.y -= 1.0f;
		}
		if (input_system.is_key_pressed(SDLK_A))
		{
			camera_direction.x -= 1.0f;
		}
		if (input_system.is_key_pressed(SDLK_D))
		{
			camera_direction.x += 1.0f;
		}

		if (glm::length(camera_direction) > 0.0f)
		{
			camera.translate(glm::normalize(camera_direction) * speed * delta_time);
		}

		// Change pos, rotation and scale every frame
		transform0.set_position(glm::vec3{ -5.0f, std::sin(elapsed_time * 2.0f) * 2.0f, 0.0f });
		transform1.rotate(90.0f * delta_time); 
		transform2.set_scale(glm::vec3{ 2.0f + std::sin(elapsed_time * 3.0f) * 0.5f });

		renderer.clear();
		renderer.draw_quad(transform0, tex, camera);
		renderer.draw_quad(transform1, tex, camera);
		renderer.draw_quad(transform2, tex, camera);

		SDL_GL_SwapWindow(window_handle);
	}
}

auto gep::game::shutdown() noexcept -> void
{
	if (gl_context != nullptr)
	{
		SDL_GL_DestroyContext(gl_context);
	}

	if (window_handle != nullptr)
	{
		SDL_DestroyWindow(window_handle);
	}

	SDL_Quit();
}