#pragma once

#include "core/types.h"

#include <glm/glm.hpp>

namespace core
{
	class EventHandler;

	class Camera
	{
	public:
		Camera(f32 pos_x, f32 pos_y, f32 pos_z, f32 up_x, f32 up_y, f32 up_z, f32 yaw, f32 pitch);

		explicit Camera(
		        glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f),
		        f32 yaw = YAW_DEFAULT, f32 pitch = PITCH_DEFAULT
		);

		glm::mat4 get_view_matrix() const;
		void      on_mouse_movement(f32 x_offset, f32 y_offset, b8 constrain_pitch = true);
		void      on_mouse_wheel_scroll(f32 mouse_wheel_direction);
		void      handle_input(const EventHandler& handler);
		void      prepare_dev_ui();

		f32 get_zoom() const
		{
			return m_zoom;
		}

	private:
		static constexpr f32 YAW_DEFAULT = -90.0f;
		static constexpr f32 PITCH_DEFAULT = 0.0f;
		static constexpr f32 SPEED_DEFAULT = 2.5f;
		static constexpr f32 SENSITIVITY_DEFAULT = 0.1f;
		static constexpr f32 ZOOM_DEFAULT = 45.0f;
		static constexpr f32 ZOOM_SPEED = 1.0f;

		// camera Attributes
		glm::vec3 m_position;
		glm::vec3 m_front;
		glm::vec3 m_up{};
		glm::vec3 m_right{};
		glm::vec3 m_world_up;
		// euler Angles
		f32       m_yaw;
		f32       m_pitch;
		// camera options
		f32       m_movement_speed;
		f32       m_mouse_sensitivity;
		f32       m_zoom;

		void update_vectors();
	};

}  // namespace core
