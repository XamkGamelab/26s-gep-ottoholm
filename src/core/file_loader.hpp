#pragma once
#include <filesystem>
#include <optional>
#include <fstream>
#include <sstream>
#include <SDL3/SDL_log.h>

namespace gep::core
{
	auto load_text_file(std::filesystem::path filepath) -> std::optional<std::string>
	{
		if (!std::filesystem::exists(filepath))
		{
			SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
				"File ""%s"" does not exist", filepath.string());
			return std::nullopt;
		}

		std::ifstream ifs{ filepath };
		if (!ifs.is_open())
		{
			SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
				"File ""%s"" cannot be opened", filepath.string());
			return std::nullopt;
		}

		std::stringstream ss;
		ss << ifs.rdbuf();
		return ss.str();
	}
}