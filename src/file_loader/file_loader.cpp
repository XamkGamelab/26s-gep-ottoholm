#include "file_loader.hpp"
#include <fstream>
#include <sstream>
#include <SDL3/SDL_log.h>

auto gep::io::file_loader::read_text_file(const std::string& path) -> std::optional<std::string>
{
	// Open the file
	std::ifstream file(path);
	if (!file.is_open())
	{
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
			"Failed to open file: %s", path.c_str());
		return std::nullopt;
	}

	std::stringstream buffer;
	buffer << file.rdbuf();
	return buffer.str();
}
