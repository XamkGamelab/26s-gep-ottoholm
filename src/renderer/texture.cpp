#include "texture.hpp"
#include <glad/gl.h>
#include <SDL3_image/SDL_image.h>

namespace gep::renderer
{
	texture::texture(std::string_view texture_path)
	{
		auto img_file = IMG_Load(texture_path.data());
		if (!img_file)
		{
			SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
				"Image failed to load: %s", SDL_GetError());
			return;
		}

		auto rgba_file = SDL_ConvertSurface(img_file, SDL_PIXELFORMAT_RGBA32);
		SDL_DestroySurface(img_file);
		if (!rgba_file)
		{
			SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
				"Image format conversion failed: %s", SDL_GetError());
			return;
		}

		SDL_FlipSurface(rgba_file, SDL_FLIP_VERTICAL);

		glGenTextures(1, &id);
		glBindTexture(GL_TEXTURE_2D, id);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexImage2D(
			GL_TEXTURE_2D, 0, GL_RGBA,
			rgba_file->w, rgba_file->h, 0,
			GL_RGBA, GL_UNSIGNED_BYTE, rgba_file->pixels);

		width = rgba_file->w;
		height = rgba_file->h;
		glGenerateMipmap(GL_TEXTURE_2D);
		SDL_DestroySurface(rgba_file);
	}

	texture::~texture()
	{
		clear();
	}

	auto texture::bind(uint16_t texture_slot_unit) const noexcept -> void
	{
		glActiveTexture(GL_TEXTURE0 + texture_slot_unit);
		glBindTexture(GL_TEXTURE_2D, id);
	}

	texture::texture(texture&& rhs) noexcept
		: id(std::exchange(rhs.id, 0)),
		width(std::exchange(rhs.width, 0)),
		height(std::exchange(rhs.height, 0))
	{
	}

	texture& texture::operator=(texture&& rhs) noexcept
	{
		if (this == &rhs)
		{
			return *this;
		}

		clear();

		id = std::exchange(rhs.id, 0);
		width = std::exchange(rhs.width, 0);
		height = std::exchange(rhs.height, 0);
	}

	auto texture::clear() noexcept -> void
	{
		if (id != 0)
		{
			glDeleteTextures(1, &id);
			id = 0;
		}
	}
}