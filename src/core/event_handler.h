#pragma once

#include "core/types.h"

#include <SDL3/SDL.h>

#include <array>
#include <functional>
#include <optional>

namespace core
{
	class EventHandler
	{
		using MouseOffsetFn = std::function<void(f32 x_offset, f32 y_offset)>;
		using MouseWheelDirectionFn = std::function<void(f32 mouse_wheel_direction)>;
		using WindowsResizingFn = std::function<void(i32 new_size_x, i32 mew_size_y)>;
		using WindowsQuitFn = std::function<void()>;
		using KeyboardInputHandlerFn = std::function<void(EventHandler& evh)>;

	public:
		struct Config
		{
			WindowsResizingFn windows_resizing_callback;
			WindowsQuitFn     windows_quit_callback;
		};

		explicit EventHandler(Config config);
		~EventHandler() = default;

		EventHandler(const EventHandler& other) = delete;
		EventHandler& operator=(const EventHandler& other) = delete;
		EventHandler(EventHandler&& other) noexcept = default;
		EventHandler& operator=(EventHandler&& other) noexcept = default;

		void register_mouse_offset_callback(const MouseOffsetFn& callback)
		{
			m_mouse_offset_fn = callback;
		}

		void register_mouse_wheel_direction_callback(const MouseWheelDirectionFn& callback)
		{
			m_mouse_wheel_direction_fn = callback;
		}

		void register_keyboard_input_handler(const KeyboardInputHandlerFn& callback)
		{
			m_keyboard_input_handler_fn = callback;
		}

		void clear_optional_callbacks();
		void collect_input();
		void process_input();
		void toggle_mouse_capture();
		b8   is_key_pressed(SDL_Keycode key_code) const;
		b8   is_key_released(SDL_Keycode key_code) const;
		b8   is_key_just_pressed(SDL_Keycode key_code) const;
		b8   is_mouse_captured() const;

	private:
		std::array<b8, SDL_SCANCODE_COUNT>    m_current_keyboard_state{};
		std::array<b8, SDL_SCANCODE_COUNT>    m_last_keyboard_state{};
		std::optional<KeyboardInputHandlerFn> m_keyboard_input_handler_fn;
		std::optional<MouseOffsetFn>          m_mouse_offset_fn;
		std::optional<MouseWheelDirectionFn>  m_mouse_wheel_direction_fn;
		WindowsResizingFn                     m_window_resizing_fn;
		WindowsQuitFn                         m_window_quit_fn;
		b8                                    m_mouse_captured;
	};
}  // namespace core
