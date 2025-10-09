#include "core/event_handler.h"

#include "dev_ui/dev_ui.h"

#include <SDL3/SDL.h>
#include <tracy/Tracy.hpp>

core::EventHandler::EventHandler(Config config)
    : m_window_resizing_fn{ std::move(config.windows_resizing_callback) }
    , m_window_quit_fn{ std::move(config.windows_quit_callback) }
    , m_mouse_captured{ true }
{
}

// TODO either handle all events here or defer them
void core::EventHandler::collect_input()
{
	ZoneScopedN("Collect Input");
	m_last_keyboard_state = m_current_keyboard_state;

	SDL_Event e;
	while (SDL_PollEvent(&e))
	{
		if (m_mouse_captured)
		{
			dev_ui::process_input(e);
		}

		switch (e.type)
		{
		case SDL_EVENT_QUIT:
			m_window_quit_fn();
			break;

		case SDL_EVENT_WINDOW_PIXEL_SIZE_CHANGED:
			m_window_resizing_fn(e.window.data1, e.window.data2);
			break;

		case SDL_EVENT_MOUSE_WHEEL:
			if (!m_mouse_captured && m_mouse_wheel_direction_fn)
			{
				(*m_mouse_wheel_direction_fn)(e.wheel.y);
			}
			break;

		case SDL_EVENT_MOUSE_MOTION:
			if (!m_mouse_captured && m_mouse_offset_fn)
			{
				(*m_mouse_offset_fn)(e.motion.xrel, -e.motion.yrel);
			}
			break;
		default:
			break;
		}
	}

	i32       num_keys;
	const b8* keyboard_state = SDL_GetKeyboardState(&num_keys);
	std::copy_n(keyboard_state, num_keys,
	            m_current_keyboard_state.data());  // store the previous state
}

void core::EventHandler::process_input()
{
	ZoneScopedN("Process Input");
	if (m_keyboard_input_handler_fn)
	{
		(*m_keyboard_input_handler_fn)(*this);
	}
}
void core::EventHandler::toggle_mouse_capture()
{
	m_mouse_captured = !m_mouse_captured;
}

b8 core::EventHandler::is_key_pressed(SDL_Keycode key_code) const
{
	return m_current_keyboard_state[SDL_GetScancodeFromKey(key_code, nullptr)];
}

b8 core::EventHandler::is_key_released(SDL_Keycode key_code) const
{
	return !is_key_pressed(SDL_GetScancodeFromKey(key_code, nullptr));
}

b8 core::EventHandler::is_key_just_pressed(SDL_Keycode key_code) const
{
	auto scancode = SDL_GetScancodeFromKey(key_code, nullptr);
	return m_current_keyboard_state[scancode] && !m_last_keyboard_state[scancode];
}
b8 core::EventHandler::is_mouse_captured() const
{
	return m_mouse_captured;
}

void core::EventHandler::clear_optional_callbacks()
{
	m_mouse_offset_fn = std::nullopt;
	m_mouse_wheel_direction_fn = std::nullopt;
	m_keyboard_input_handler_fn = std::nullopt;
}
