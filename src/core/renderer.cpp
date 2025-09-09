#include "core/renderer.h"

#include "core/event_handler.h"
#include "core/filesystem.h"

#include <glad/gl.h>
#include <imgui/imgui.h>
#include <spdlog/spdlog.h>

#include <stb/stb_image.h>

namespace core
{
Renderer::Renderer()
        : m_shader{ CoreShaderFile("vertex_shader.vert"), CoreShaderFile("fragment_shader.frag") }
{
	glGenVertexArrays(1, &m_vao);
	glGenVertexArrays(1, &m_vao2);
	glGenBuffers(1, &m_vbo);
	glGenBuffers(1, &m_vbo2);
	// glGenBuffers(1, &m_ebo);
}

Renderer::~Renderer()
{
	glDeleteVertexArrays(1, &m_vao);
	glDeleteVertexArrays(1, &m_vao2);
	glDeleteBuffers(1, &m_vbo);
	glDeleteBuffers(1, &m_vbo2);
	// glDeleteBuffers(1, &m_ebo);
}

void Renderer::setup_rendering() const
{
	// clang-format off
	static constexpr std::array VERTICES = {
		// first triangle
		-0.9f, -0.5f, 0.0f,  // left 
		-0.0f, -0.5f, 0.0f,  // right
		-0.45f, 0.5f, 0.0f,  // top 
		
   };

	static constexpr std::array VERTICES2 = {
		// second triangle
		0.0f, -0.5f, 0.0f,  // left
		0.9f, -0.5f, 0.0f,  // right
		0.45f, 0.5f, 0.0f   // top 
		
   };

	// static constexpr std::array INDICES = {  // note that we start from 0!
	// 	0, 1, 3,   // first triangle
	// 	1, 2, 3    // second triangle
	// };
	// clang-format on
	// triangle 1
	glBindVertexArray(m_vao);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(VERTICES), VERTICES.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
	glEnableVertexAttribArray(0);

	//triangle 2
	glBindVertexArray(m_vao2);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo2);
	glBufferData(GL_ARRAY_BUFFER, sizeof(VERTICES2), VERTICES2.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
	glEnableVertexAttribArray(0);
	
	m_shader.use();

	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
}

void Renderer::render() const
{
	// wireframe on/off
	glPolygonMode(GL_FRONT_AND_BACK, m_is_wireframe_active ? GL_LINE : GL_FILL);
	
	// clear the screen if not drawing in full to avoid flickering
	glClear(GL_COLOR_BUFFER_BIT);

	m_shader.use();
	glBindVertexArray(m_vao);
	glDrawArrays(GL_TRIANGLES, 0, 3);

	glBindVertexArray(m_vao2);
	glDrawArrays(GL_TRIANGLES, 0, 3);
}

void Renderer::prepare_dev_ui()
{
	ImGui::Begin("Learning OpenGL");

	// shorcuts table
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
	return false;
}
}  // namespace core
