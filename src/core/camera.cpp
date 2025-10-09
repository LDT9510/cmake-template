#include "camera.h"

#include "core/event_handler.h"
#include "core/timing.h"
#include "glm/gtc/type_ptr.hpp"

#include <algorithm>
#include <imgui/imgui.h>
#include <glm/gtc/matrix_transform.hpp>

namespace core
{
	enum class CameraMovement : u8
	{
		None,
		Forward,
		Backward,
		Left,
		Right
	};

	Camera::Camera(f32 pos_x, f32 pos_y, f32 pos_z, f32 up_x, f32 up_y, f32 up_z, f32 yaw, f32 pitch)
	        : m_position{ glm::vec3(pos_x, pos_y, pos_z) }
	        , m_front{ glm::vec3(0.0f, 0.0f, -1.0f) }
	        , m_world_up{ glm::vec3(up_x, up_y, up_z) }
	        , m_yaw{ yaw }
	        , m_pitch{ pitch }
	        , m_movement_speed{ SPEED_DEFAULT }
	        , m_mouse_sensitivity{ SENSITIVITY_DEFAULT }
	        , m_zoom{ ZOOM_DEFAULT }
	{
		update_vectors();
	}

	Camera::Camera(glm::vec3 position, glm::vec3 up, f32 yaw, f32 pitch)
	        : Camera(position.x, position.y, position.z, up.x, up.y, up.z, yaw, pitch)
	{
	}

	void Camera::update_vectors()
	{
		glm::vec3 front;
		front.x = glm::cos(glm::radians(m_yaw)) * glm::cos(glm::radians(m_pitch));
		front.y = glm::sin(glm::radians(m_pitch));
		front.z = glm::sin(glm::radians(m_yaw)) * glm::cos(glm::radians(m_pitch));
		m_front = normalize(front);
		m_right = normalize(cross(front, m_world_up));
		m_up = normalize(cross(m_right, front));
	}

	glm::mat4 Camera::get_view_matrix() const
	{
		return lookAt(m_position, m_position + m_front, m_up);
	}

	void Camera::on_mouse_movement(f32 x_offset, f32 y_offset, b8 constrain_pitch)
	{
		m_yaw += x_offset * m_mouse_sensitivity;
		m_pitch += y_offset * m_mouse_sensitivity;

		// make sure that when pitch is out of bounds, screen doesn't get flipped
		if (constrain_pitch)
		{
			m_pitch = std::clamp(m_pitch, -89.0f, 89.0f);
		}

		update_vectors();
	}

	void Camera::on_mouse_wheel_scroll(f32 mouse_wheel_direction)
	{
		m_zoom -= ZOOM_SPEED * mouse_wheel_direction;
		m_zoom = std::clamp(m_zoom, 1.0f, 45.0f);
	}

	void Camera::handle_input(const EventHandler& handler)
	{
		CameraMovement movement_direction = CameraMovement::None;

		if (handler.is_key_pressed(SDLK_W))
		{
			movement_direction = CameraMovement::Forward;
		}
		if (handler.is_key_pressed(SDLK_S))
		{
			movement_direction = CameraMovement::Backward;
		}
		if (handler.is_key_pressed(SDLK_A))
		{
			movement_direction = CameraMovement::Left;
		}
		if (handler.is_key_pressed(SDLK_D))
		{
			movement_direction = CameraMovement::Right;
		}

		f32 velocity = m_movement_speed * timing::get_delta_time();
		switch (movement_direction)
		{
		case CameraMovement::Forward:
			m_position += m_front * velocity;
			break;
		case CameraMovement::Backward:
			m_position -= m_front * velocity;
			break;
		case CameraMovement::Left:
			m_position -= m_right * velocity;
			break;
		case CameraMovement::Right:
			m_position += m_right * velocity;
			break;
		default:
			break;
		}
	}

	void Camera::prepare_dev_ui()
	{
		if (ImGui::CollapsingHeader("Camera"))
		{
			ImGui::SliderAngle("FOV", &m_zoom, 10.0f, 120.0f);
			ImGui::SliderFloat("Camera Speed", &m_movement_speed, 1.0f, 100.0f);
			ImGui::InputFloat3("Camera Position", glm::value_ptr(m_position));
		}
	}
}  // namespace core
