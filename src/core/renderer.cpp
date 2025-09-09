#include "core/renderer.h"

#include "core/event_handler.h"
#include "core/filesystem.h"
#include "core/timing.h"

#include <glad/gl.h>
#include <imgui/imgui.h>
#include <spdlog/spdlog.h>
#include <stb/stb_image.h>
#include <glm/glm.hpp>

namespace core
{
Renderer::Renderer()
        : m_shader{ CoreShaderFile("vertex_shader.vert"), CoreShaderFile("fragment_shader.frag") }
{
	glGenVertexArrays(1, &m_vao);
	glGenBuffers(1, &m_vbo);
	// glGenBuffers(1, &m_ebo);
}

Renderer::~Renderer()
{
	glDeleteVertexArrays(1, &m_vao);
	glDeleteBuffers(1, &m_vbo);
	// glDeleteBuffers(1, &m_ebo);
}

void Renderer::setup_rendering() const
{
	// clang-format off
	static constexpr std::array VERTICES = {
		// positions         // colors
		0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,   // bottom right
	   -0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,   // bottom left
		0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f    // top 
   };

	// static constexpr std::array INDICES = {  // note that we start from 0!
	// 	0, 1, 3,   // first triangle
	// 	1, 2, 3    // second triangle
	// };
	// clang-format on

	glBindVertexArray(m_vao);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(VERTICES), VERTICES.data(), GL_STATIC_DRAW);
	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3* sizeof(float)));
	glEnableVertexAttribArray(1);

	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
}

void Renderer::render() const
{
	// wireframe on/off
	glPolygonMode(GL_FRONT_AND_BACK, m_is_wireframe_active ? GL_LINE : GL_FILL);

	// clear the screen if not drawing in full to avoid flickering
	glClear(GL_COLOR_BUFFER_BIT);

	// uniform update
	float time_value = timing::get_sdl_elapsed_seconds();
	float green_value = (glm::sin(time_value) / 2.0f) + 0.5f;
	int   vertex_color_location = glGetUniformLocation(m_shader.m_program_id, "ourColor");
	glUseProgram(m_shader.m_program_id);  // basically m_shader.use()
	glUniform4f(vertex_color_location, 0.0f, green_value, 0.0f, 1.0f);
	
	glBindVertexArray(m_vao);
	glDrawArrays(GL_TRIANGLES, 0, 3);
}

void Renderer::prepare_dev_ui()
{
	ImGui::Begin("Learning OpenGL");

	if (ImGui::CollapsingHeader("Global Shortcuts")) {
		constexpr ImGuiTableFlags table_flags = ImGuiTableFlags_RowBg | ImGuiTableFlags_Borders;
		constexpr auto            yellow = ImVec4(1.0f, 1.0f, 0.0f, 1.0f);
		if (ImGui::BeginTable("shortcuts", 2, table_flags)) {
			ImGui::TableSetupColumn("Shortcut", ImGuiTableColumnFlags_WidthFixed);
			ImGui::TableSetupColumn("Description", ImGuiTableColumnFlags_WidthFixed);
			ImGui::TableHeadersRow();
			ImGui::TableNextRow();
			ImGui::TableNextColumn();
			ImGui::Indent();
			{
				ImGui::TextColored(yellow, "U");
				ImGui::TableNextColumn();
				ImGui::Text("Enables wireframe mode");
			}
			ImGui::EndTable();
		}
	}

	if (ImGui::Button("Reload shaders")) {
		if (!m_is_shader_reloading) {
			if (reload_shaders()) {  // NOLINT(*-branch-clone)
				SPDLOG_INFO("All Shaders reloaded OK.");
			} else {
				SPDLOG_ERROR("Error reloading shaders.");
			}
		} else {
			SPDLOG_INFO("Already compiling, please wait...");
		}
	}

	ImGui::End();
}

void Renderer::handle_input(const EventHandler& event_handler)
{
	if (event_handler.is_key_just_pressed(SDLK_U)) {
		m_is_wireframe_active = !m_is_wireframe_active;
	}
}

b8 Renderer::reload_shaders()
{
	m_is_shader_reloading = true;
	m_shader = { CoreShaderFile("vertex_shader.vert"), CoreShaderFile("fragment_shader.frag") };
	m_is_shader_reloading = false;

	return m_shader.is_valid();
}
}  // namespace core
