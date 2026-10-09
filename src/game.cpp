#include "game.hpp"
#include "input/input_system.hpp"
#include "time/time_manager.hpp"

#include <string>
#include <SDL3/SDL_init.h>
#include <SDL3/SDL_log.h>
#include <SDL3/SDL_video.h>
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_keycode.h>
#include <SDL3/SDL_render.h>
#include <SDL3_image/SDL_image.h>

#include <glad/gl.h>

auto gep::game::init() noexcept -> bool
{
	if(!SDL_Init(SDL_INIT_VIDEO))
	{
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, 
			"SDL_Init failed: %s", SDL_GetError());
		return false;
	}

	// configure SDL to request OpenGL 4.6
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

	handle = SDL_CreateWindow("Gep", 1280, 720, SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL);
	if (handle == nullptr)
	{
		SDL_LogError(SDL_LOG_CATEGORY_VIDEO,
			"SDL_CreateWindow: %s", SDL_GetError());
		return false;
	}

	gl_context = SDL_GL_CreateContext(handle);
	if (gl_context == nullptr)
	{
		SDL_LogError(SDL_LOG_CATEGORY_VIDEO, "SDL_GL_CreateContext: %s", SDL_GetError());
		return false;
	}

	if (gladLoadGL((GLADloadfunc)SDL_GL_GetProcAddress) == 0)
	{
		SDL_LogError(SDL_LOG_CATEGORY_VIDEO, "Failed to initialize GLAD");
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

	// Singletons
	auto& input_system = input::input_system::instance();
	auto& time_manager = time::time_manager::instance();

	// Define background color for changing them
	SDL_Color background{ 30, 30, 30, 255 };

	while (is_running)
	{
		time_manager.tick();
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

		// move speed times delta time so movement is same on different computers and not changing by performance differences
		float delta_time = time_manager.get_delta_time();
		float move_speed = 100.0f;

		if (input_system.is_key_pressed(SDLK_W))
		{
			image_y -= move_speed * delta_time;
		}
		if (input_system.is_key_pressed(SDLK_A))
		{
			image_x -= move_speed * delta_time;
		}
		if (input_system.is_key_pressed(SDLK_S))
		{
			image_y += move_speed * delta_time;
		}
		if (input_system.is_key_pressed(SDLK_D))
		{
			image_x += move_speed * delta_time;
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

		const char* vertexShaderSource = "#version 460 core\n"
			"layout (location = 0) in vec3 in_pos;\n"
			"void main()\n"
			"{\n"
			"   gl_Position = vec4(in_pos.x, in_pos.y, in_pos.z, 1.0);\n"
			"}\0";

		const char* fragShaderSource = "#version 460 core\n"
			"out ve4 FragColor;\n"
			"void main()\n"
			"{\n"
			"   FragColor = vec4(1.0f 0.5f, 0.2f, 1.0);\n"
			"}\0";

		// Get shader objects
		uint32_t vertexShader, fragmentShader;
		vertexShader = glCreateShader(GL_VERTEX_SHADER);
		fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

		// Give shader objects thir source codes
		glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
		glCompileShader(vertexShader);

		int32_t status = 0;
		glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &status);
		if (status == 0) {
			int32_t log_lenght;
			glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &log_lenght);
			std::string log = std::string((std::size_t)log_lenght, ' ');
			glGetShaderInfoLog(vertexShader, log_lenght, nullptr, &log[0]);
			SDL_LogError(SDL_LOG_CATEGORY_GPU,
				"Vertex shader compile failed: %s", log.c_str());
		}

		// Give shader objects thir source codes 
		glShaderSource(fragmentShader, 1, &fragShaderSource, NULL);
		glCompileShader(fragmentShader);

		status = 0;
		glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &status);
		if (status == 0) {
			int32_t log_lenght;
			glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &log_lenght);
			std::string log = std::string((std::size_t)log_lenght, ' ');
			glGetShaderInfoLog(fragmentShader, log_lenght, nullptr, &log[0]);
			SDL_LogError(SDL_LOG_CATEGORY_GPU,
				"Fragment shader compile failed: %s", log.c_str());
		}

		// Create shader programn
		uint32_t shaderProgram = glCreateProgram();
		// Attach shaders to the shader program
		glAttachShader(shaderProgram, vertexShader);
		glAttachShader(shaderProgram, fragmentShader);
		// Link shader program
		glLinkProgram(shaderProgram);
		status = 0;
		glGetProgramiv(shaderProgram, GL_LINK_STATUS, &status);
		if (status == 0) {
			int32_t log_lenght;
			glGetShaderiv(shaderProgram, GL_INFO_LOG_LENGTH, &log_lenght);
			std::string log = std::string((std::size_t)log_lenght, ' ');
			glGetProgramInfoLog(shaderProgram, log_lenght, nullptr, &log[0]);
			SDL_LogError(SDL_LOG_CATEGORY_GPU,
				"Shader program linking failed: %s", log.c_str());
		}

		// Delete shaders
		glDeleteShader(vertexShader);  // Deallocation
		glDeleteShader(fragmentShader); // Deallocation
		// Use shader program
		glUseProgram(shaderProgram);

		// Triangle
		float vertices[] = {
			// X    Y
		    -0.5f, -0.5f,
			0.5f,  -0.5f,
			0.0f,   0.5f,
		};

		
		uint32_t vao, vbo;
		// vertex array object --> larger state machine inside opengl
		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);

		// vertex buffer object --> takes vertex data
		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		uint32_t element_size = 2 * sizeof(float);
		uint32_t vertex_count = sizeof(vertices) / element_size;
		glVertexAttribPointer(0, 
			vertex_count, GL_FLOAT, GL_FALSE, element_size, (void*)0);
		glEnableVertexAttribArray(0);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		// clean up
		glBindVertexArray(0);
		glDeleteBuffers(1, &vbo);
		glDeleteVertexArrays(1, &vao);


		SDL_GL_SwapWindow(handle);
	}
}

auto gep::game::shutdown() noexcept -> void
{
	if (handle != nullptr)
	{
		SDL_DestroyWindow(handle);
	}

	if (gl_context != nullptr)
	{
		SDL_GL_DestroyContext(gl_context);
	}

	SDL_Quit();
}