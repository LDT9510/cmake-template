#include "camera.hpp"

#include "core/event_handler.hpp"
#include "core/timing.hpp"
#include "glm/gtc/type_ptr.hpp"

#include <imgui/imgui.h>

#include <algorithm>
#include <glm/gtc/matrix_transform.hpp>

namespace
{
	// Custom implementation of the LookAt function
	static glm::mat4 custom_look_at_matrix(
	    const glm::vec3& position, const glm::vec3& target, const glm::vec3& world_up)
	{
		// 1. Position = known
		// 2. Calculate cameraDirection
		glm::vec3 z_axis = glm::normalize(position - target);
		// 3. Get positive right axis vector
		glm::vec3 x_axis = glm::normalize(glm::cross(glm::normalize(world_up), z_axis));
		// 4. Calculate camera up vector
		glm::vec3 y_axis = glm::cross(z_axis, x_axis);

		// Create translation and rotation matrix
		// In glm we access elements as mat[col][row] due to column-major layout
		glm::mat4 translation = { 1.0f };  // Identity matrix by default
		translation[3][0] = -position.x;  // Fourth column, first row
		translation[3][1] = -position.y;
		translation[3][2] = -position.z;
		glm::mat4 rotation = { 1.0f };
		rotation[0][0] = x_axis.x;  // First column, first row
		rotation[1][0] = x_axis.y;
		rotation[2][0] = x_axis.z;
		rotation[0][1] = y_axis.x;  // First column, second row
		rotation[1][1] = y_axis.y;
		rotation[2][1] = y_axis.z;
		rotation[0][2] = z_axis.x;  // First column, third row
		rotation[1][2] = z_axis.y;
		rotation[2][2] = z_axis.z;

		// Return lookAt matrix as combination of translation and rotation matrix
		return rotation * translation;  // Remember to read from right to left (first translation
		                                // then rotation)
	}
}  // namespace

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
    , m_fps_mode{ false }
    , m_use_custom_look_at{ false }
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
	using LookAtFunc = glm::mat4 (*)(
	    const glm::vec3& position, const glm::vec3& target, const glm::vec3& world_up);

	LookAtFunc look_at_func = m_use_custom_look_at ? &custom_look_at_matrix :
	                                                 &glm::lookAt<float, glm::defaultp>;

	return look_at_func(m_position, m_position + m_front, m_up);
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

	f32 current_y_pos = m_position.y;

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

	if (m_fps_mode)
	{
		m_position.y = current_y_pos;
	}
}

void core::Camera::prepare_dev_ui()
{
	if (ImGui::CollapsingHeader("Camera"))
	{
		ImGui::SliderAngle("FOV", &m_zoom, 10.0f, 120.0f);
		ImGui::SliderFloat("Camera Speed", &m_movement_speed, 1.0f, 100.0f);
		ImGui::InputFloat3("Camera Position", glm::value_ptr(m_position));
		ImGui::Checkbox("Custom look at matrix", &m_use_custom_look_at);
		ImGui::Checkbox("FPS mode (lock XZ plane)", &m_fps_mode);
	}
}
