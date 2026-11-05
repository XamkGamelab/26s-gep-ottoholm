#include "shader.hpp"
#include "..\core\file_loader.hpp"
#include <glad/gl.h>
#include <glm/gtc/type_ptr.hpp>
#include <optional>
#include <utility>


namespace gep::renderer
{
	shader::shader(std::string_view v_path, std::string_view f_path)
	{
		uint32_t vertexShader, fragmentShader;
		vertexShader = glCreateShader(GL_VERTEX_SHADER);

		auto vertex_source = core::load_text_file(v_path);
		if (!vertex_source.has_value())
		{
			return;
		}

		auto success = compile_shader(vertexShader, vertex_source.value().c_str());

		fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		auto frag_source = core::load_text_file(f_path);
		if (!frag_source.has_value())
		{
			return;
		}

		success = compile_shader(fragmentShader, frag_source.value().c_str());

		// Shader program creation ---> OpenGL object
		id = glCreateProgram(); // Allocation
		// Attach shaders to the shader program
		glAttachShader(id, vertexShader);
		glAttachShader(id, fragmentShader);
		// Link shader program
		glLinkProgram(id);
		int32_t status = 0;
		glGetProgramiv(id, GL_LINK_STATUS, &status);
		if (status == 0)
		{
			int32_t log_length;
			glGetProgramiv(id, GL_INFO_LOG_LENGTH, &log_length);
			std::string log = std::string((std::size_t)log_length, ' ');
			glGetProgramInfoLog(id, log_length, nullptr, &log[0]);
			SDL_LogError(SDL_LOG_CATEGORY_GPU,
				"Shader program linking failed: %s", log.c_str());
		}

		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);
	}

	shader::~shader()
	{
		clear();
	}

	auto shader::bind() const noexcept -> void
	{
		glUseProgram(id);
	}

	auto shader::clear() -> void
	{
		if (id != 0)
		{
			glDeleteProgram(id);
			id = 0;
		}
	}

	auto shader::set_int(const char* name, int32_t value) const noexcept -> void
	{
		int32_t location = -1;
		if (!try_get_uniform_location(name, location))
		{
			return;
		}

		glUniform1i(location, value);
	}

	auto shader::set_mat4x4(const char* name, const glm::mat4x4& value) const noexcept -> void
	{
		int32_t location = -1;
		if (!try_get_uniform_location(name, location))
		{
			return;
		}

		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value));
	}

	shader::shader(shader&& rhs) noexcept
		: id(std::exchange(rhs.id, 0))
	{
	}

	shader& shader::operator=(shader&& rhs) noexcept
	{
		if (this == &rhs)
		{
			return *this;
		}

		clear();

		id = std::exchange(rhs.id, 0);
	}

	auto shader::compile_shader(uint32_t shader, const char* source) const -> bool
	{
		glShaderSource(shader, 1, &source, nullptr);
		glCompileShader(shader);

		int32_t status = 0;
		glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
		if (status != 0)
		{
			return true;
		}

		int32_t log_length;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &log_length);
		std::string log = std::string((std::size_t)log_length, ' ');
		glGetShaderInfoLog(shader, log_length, nullptr, &log[0]);
		SDL_LogError(SDL_LOG_CATEGORY_GPU,
			"Frag shader compile failed: %s", log.c_str());

		return false;
	}

	auto shader::try_get_uniform_location(const char* name, int32_t& location) const noexcept -> bool
	{
		location = glGetUniformLocation(id, name);
		if (location < 0)
		{
			SDL_LogError(SDL_LOG_CATEGORY_GPU,
				"Shader %i could not find uniform ""%s""", id, name);
			return false;
		}

		return true;
	}
}