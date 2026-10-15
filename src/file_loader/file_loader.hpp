#pragma once
#include <optional>
#include <string>

namespace gep::io
{
	class [[nodiscard]] file_loader
	{
	public:
		// Read whole file into a string
		[[nodiscard]]
		static auto read_text_file(const std::string& path) -> std::optional<std::string>;
	};
}
