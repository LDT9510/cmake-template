#include "core/event_handler.h"

#include "dev_ui/dev_ui.h"

#include <SDL3/SDL.h>
#include <tracy/Tracy.hpp>

namespace core
{
	EventHandler::EventHandler(Config config)
	        : m_window_resizing_fn{ std::move(config.windows_resizing_callback) }
	        , m_window_quit_fn{ std::move(config.windows_quit_callback) }
	{
	}

	// TODO either handle all events here or defer them
	void EventHandler::collect_input()
	{
		ZoneScopedN("Collect Input");
		m_last_keyboard_state = m_current_keyboard_state;

		SDL_Event e;
		while (SDL_PollEvent(&e))
		{
			dev_ui::process_input(e);

			switch (e.type)
			{
			case SDL_EVENT_QUIT:
				m_window_quit_fn();
				break;

			case SDL_EVENT_WINDOW_PIXEL_SIZE_CHANGED:
				m_window_resizing_fn(e.window.data1, e.window.data2);
				break;

			case SDL_EVENT_MOUSE_WHEEL:
				if (m_mouse_wheel_direction_fn)
				{
					(*m_mouse_wheel_direction_fn)(e.wheel.y);
				}
				break;

			case SDL_EVENT_MOUSE_MOTION:
				if (m_mouse_offset_fn)
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
		std::copy_n(
		        keyboard_state, num_keys,
		        m_current_keyboard_state.data()
		);  // store the previous state
	}

	void EventHandler::process_input() const
	{
		ZoneScopedN("Process Input");
		if (m_keyboard_input_handler_fn)
		{
			(*m_keyboard_input_handler_fn)(*this);
		}
	}

	b8 EventHandler::is_key_pressed(SDL_Keycode key_code) const
	{
		return m_current_keyboard_state[SDL_GetScancodeFromKey(key_code, nullptr)];
	}

	b8 EventHandler::is_key_released(SDL_Keycode key_code) const
	{
		return !is_key_pressed(SDL_GetScancodeFromKey(key_code, nullptr));
	}

	b8 EventHandler::is_key_just_pressed(SDL_Keycode key_code) const
	{
		auto scancode = SDL_GetScancodeFromKey(key_code, nullptr);
		return m_current_keyboard_state[scancode] && !m_last_keyboard_state[scancode];
	}

	void EventHandler::clear_optional_callbacks()
	{
		m_mouse_offset_fn = std::nullopt;
		m_mouse_wheel_direction_fn = std::nullopt;
		m_keyboard_input_handler_fn = std::nullopt;
	}
}  // namespace core
