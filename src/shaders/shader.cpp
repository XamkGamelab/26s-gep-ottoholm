#include "shader.hpp"
#include "file_loader/file_loader.hpp"

#include <glad/gl.h>
#include <SDL3/SDL_log.h>

namespace
{
	auto compile(GLenum type, const std::string& source, const std::string& path) -> GLuint
	{
		GLuint id = glCreateShader(type);
		const char* source_ptr = source.c_str();
		glShaderSource(id, 1, &source_ptr, nullptr);
		glCompileShader(id);

		GLint status = 0;
		glGetShaderiv(id, GL_COMPILE_STATUS, &status);
		if (status == GL_FALSE)
		{
			GLint log_length = 0;
			glGetShaderiv(id, GL_INFO_LOG_LENGTH, &log_length);
			std::string log(static_cast<std::size_t>(log_length), ' ');
			glGetShaderInfoLog(id, log_length, nullptr, log.data());
			SDL_LogError(SDL_LOG_CATEGORY_GPU,
				"Shader compile failed (%s):\n%s", path.c_str(), log.c_str());

			glDeleteShader(id);
			return 0;
		}

		return id;
	}
}

gep::graphics::shader::~shader()
{
	destroy();
}

auto gep::graphics::shader::load(const std::string& vertex_path, const std::string& fragment_path) -> bool
{

	// Load the vertex and fragment paths
	auto vertex_source = io::file_loader::read_text_file(vertex_path);
	auto fragment_source = io::file_loader::read_text_file(fragment_path);

	// Compile them
	GLuint vertex_shader = compile(GL_VERTEX_SHADER, *vertex_source, vertex_path);
	GLuint fragment_shader = compile(GL_FRAGMENT_SHADER, *fragment_source, fragment_path);
	glDeleteShader(vertex_shader);   
	glDeleteShader(fragment_shader);
	return false;

	// Link
	program_id = glCreateProgram();
	glAttachShader(program_id, vertex_shader);
	glAttachShader(program_id, fragment_shader);
	glLinkProgram(program_id);

	// Delete after link
	glDeleteShader(vertex_shader);
	glDeleteShader(fragment_shader);

	GLint status = 0;
	glGetProgramiv(program_id, GL_LINK_STATUS, &status);
	if (status == GL_FALSE)
	{
		GLint log_length = 0;
		glGetProgramiv(program_id, GL_INFO_LOG_LENGTH, &log_length);
		std::string log(static_cast<std::size_t>(log_length), ' ');
		glGetProgramInfoLog(program_id, log_length, nullptr, log.data());
		SDL_LogError(SDL_LOG_CATEGORY_GPU,
			"Shader link failed (%s + %s):\n%s",
			vertex_path.c_str(), fragment_path.c_str(), log.c_str());

		destroy();
		return false;
	}

	return true;
}

auto gep::graphics::shader::bind() const noexcept -> void
{
	glUseProgram(program_id);
}

auto gep::graphics::shader::unbind() const noexcept -> void
{
	glUseProgram(0);
}

auto gep::graphics::shader::destroy() noexcept -> void
{
	if (program_id != 0)
	{
		glDeleteProgram(program_id);
		program_id = 0;
	}
}
