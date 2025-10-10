#include "camera.hpp"

#include "core/event_handler.hpp"
#include "core/timing.hpp"
#include "glm/gtc/type_ptr.hpp"

#include <imgui/imgui.h>

#include <algorithm>
#include <glm/gtc/matrix_transform.hpp>

enum class CameraMovement : u8
{
	NONE,
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT
};

core::Camera::Camera(
    f32 pos_x, f32 pos_y, f32 pos_z, f32 up_x, f32 up_y, f32 up_z, f32 yaw, f32 pitch)
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

core::Camera::Camera(glm::vec3 position, glm::vec3 up, f32 yaw, f32 pitch)
    : Camera(position.x, position.y, position.z, up.x, up.y, up.z, yaw, pitch)
{
}

void core::Camera::update_vectors()
{
	glm::vec3 front;
	front.x = glm::cos(glm::radians(m_yaw)) * glm::cos(glm::radians(m_pitch));
	front.y = glm::sin(glm::radians(m_pitch));
	front.z = glm::sin(glm::radians(m_yaw)) * glm::cos(glm::radians(m_pitch));
	m_front = normalize(front);
	m_right = normalize(cross(front, m_world_up));
	m_up = normalize(cross(m_right, front));
}

glm::mat4 core::Camera::get_view_matrix() const
{
	return lookAt(m_position, m_position + m_front, m_up);
}

void core::Camera::on_mouse_movement(f32 x_offset, f32 y_offset, b8 constrain_pitch)
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

void core::Camera::on_mouse_wheel_scroll(f32 mouse_wheel_direction)
{
	m_zoom -= ZOOM_SPEED * mouse_wheel_direction;
	m_zoom = std::clamp(m_zoom, 1.0f, 45.0f);
}

void core::Camera::handle_input(const EventHandler& handler)
{
	CameraMovement movement_direction = CameraMovement::NONE;

	if (handler.is_key_pressed(SDLK_W))
	{
		movement_direction = CameraMovement::FORWARD;
	}
	if (handler.is_key_pressed(SDLK_S))
	{
		movement_direction = CameraMovement::BACKWARD;
	}
	if (handler.is_key_pressed(SDLK_A))
	{
		movement_direction = CameraMovement::LEFT;
	}
	if (handler.is_key_pressed(SDLK_D))
	{
		movement_direction = CameraMovement::RIGHT;
	}

	f32 velocity = m_movement_speed * timing::get_delta_time();
	switch (movement_direction)
	{
	case CameraMovement::FORWARD:
		m_position += m_front * velocity;
		break;
	case CameraMovement::BACKWARD:
		m_position -= m_front * velocity;
		break;
	case CameraMovement::LEFT:
		m_position -= m_right * velocity;
		break;
	case CameraMovement::RIGHT:
		m_position += m_right * velocity;
		break;
	default:
		break;
	}
}

void core::Camera::prepare_dev_ui()
{
	if (ImGui::CollapsingHeader("Camera"))
	{
		ImGui::SliderAngle("FOV", &m_zoom, 10.0f, 120.0f);
		ImGui::SliderFloat("Camera Speed", &m_movement_speed, 1.0f, 100.0f);
		ImGui::InputFloat3("Camera Position", glm::value_ptr(m_position));
	}
}
