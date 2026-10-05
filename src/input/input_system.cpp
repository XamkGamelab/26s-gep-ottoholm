#include "input_system.hpp"
#include <SDL3/SDL.h>


auto gep::input::input_system::process_input(const SDL_Event& ev) noexcept -> void
{
	auto keycode = ev.key.key;
	auto mouse_button_index = ev.button.button;

	if (ev.type == SDL_EVENT_KEY_DOWN)
	{
		keys_down.insert(keycode);
		keys_pressed.insert(keycode);
	}
	else if (ev.type == SDL_EVENT_KEY_UP)
	{
		keys_up.insert(keycode);
		keys_pressed.erase(keycode);
	}
	else if (ev.type == SDL_EVENT_MOUSE_BUTTON_DOWN)
	{
		mouse_down.insert(mouse_button_index);
		mouse_pressed.insert(mouse_button_index);
	}
	else if (ev.type == SDL_EVENT_MOUSE_BUTTON_UP)
	{
		mouse_up.insert(mouse_button_index);
		mouse_pressed.erase(mouse_button_index);
	}
}

auto gep::input::input_system::update() noexcept -> void
{
	keys_down.clear();
	keys_up.clear();

	mouse_down.clear();
	mouse_up.clear();
}

auto gep::input::input_system::is_key_down(uint32_t keycode) const noexcept -> bool
{
	return keys_down.contains(keycode);
}

auto gep::input::input_system::is_key_up(uint32_t keycode) const noexcept -> bool
{
	return keys_up.contains(keycode);
}

auto gep::input::input_system::is_key_pressed(uint32_t keycode) const noexcept -> bool
{
	return keys_pressed.contains(keycode);
}

auto gep::input::input_system::is_mouse_down(uint8_t mouse_button_index) const noexcept -> bool
{
	return mouse_down.contains(mouse_button_index);
}

auto gep::input::input_system::is_mouse_up(uint8_t mouse_button_index) const noexcept -> bool
{
	return mouse_up.contains(mouse_button_index);
}

auto gep::input::input_system::is_mouse_pressed(uint8_t mouse_button_index) const noexcept -> bool
{
	return mouse_pressed.contains(mouse_button_index);
}