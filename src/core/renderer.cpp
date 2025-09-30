#include "core/renderer.h"

#include "core/event_handler.h"
#include "core/filesystem.h"
#include "core/timing.h"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "utils/texture_utils.h"

#include <span>
#include <glad/gl.h>
#include <imgui/imgui.h>
#include <spdlog/spdlog.h>
#include <stb/stb_image.h>
#include <glm/glm.hpp>

namespace core
{

static constexpr auto plane()
{
	// clang-format off
	return std::to_array({
		// positions          // colors           // texture coords
		0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
		0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
	   -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
	   -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // top left 
   });
	// clang-format on
}

static constexpr auto cube()
{
	// clang-format off
	return std::to_array({
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
	 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
});
	// clang-format on
}

static constexpr std::array CUBE_POSITIONS = {
	glm::vec3( 0.0f,  0.0f,  0.0f), 
	glm::vec3( 2.0f,  5.0f, -15.0f), 
	glm::vec3(-1.5f, -2.2f, -2.5f),  
	glm::vec3(-3.8f, -2.0f, -12.3f),  
	glm::vec3( 2.4f, -0.4f, -3.5f),  
	glm::vec3(-1.7f,  3.0f, -7.5f),  
	glm::vec3( 1.3f, -2.0f, -2.5f),  
	glm::vec3( 1.5f,  2.0f, -2.5f), 
	glm::vec3( 1.5f,  0.2f, -1.5f), 
	glm::vec3(-1.3f,  1.0f, -1.5f)  
};

Renderer::Renderer(const core::Window& window)
        : m_shader{ CoreShaderFile("vertex_shader.vert"), CoreShaderFile("fragment_shader.frag") }
        , m_window{ &window }
{
	glGenVertexArrays(1, &m_vao);
	glGenBuffers(1, &m_vbo);
	glGenBuffers(1, &m_ebo);
	glGenTextures(1, &m_texture);
	glGenTextures(1, &m_texture2);
}

Renderer::~Renderer()
{
	glDeleteVertexArrays(1, &m_vao);
	glDeleteBuffers(1, &m_vbo);
	glDeleteBuffers(1, &m_ebo);
	glDeleteTextures(1, &m_texture);
	glDeleteTextures(1, &m_texture2);
}

void Renderer::setup_rendering()
{
	static constexpr auto VERTICES = cube();

	// not used
	// static constexpr std::array INDICES = {
	// 	0, 1, 3,  // first triangle
	// 	1, 2, 3  // second triangle
	// };

	glBindVertexArray(m_vao);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(VERTICES), VERTICES.data(), GL_STATIC_DRAW);

	// glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
	// glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(INDICES), INDICES.data(), GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(f32), nullptr);
	glEnableVertexAttribArray(0);

	// texture coordinates attribute
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(f32),
	                      (const void*)(3 * sizeof(f32)));  // NOLINT(*-no-int-to-ptr)
	glEnableVertexAttribArray(1);

	// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex
	// attribute's bound vertex buffer object so afterward we can safely unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object
	// IS stored in the VAO; keep the EBO bound. glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	// You can unbind the VAO afterward so other VAO calls won't accidentally modify this VAO,
	// but this rarely happens. Modifying other VAOs requires a call to glBindVertexArray anyway
	// so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
	glBindVertexArray(0);

	load_texture("container.jpg", m_texture);
	load_texture("awesomeface.png", m_texture2);

	m_shader.use();  // activate before setting uniforms
	// inform OpenGL to which texture unit each shader sampler belongs to
	m_shader.set_int32("texture1", 0);
	m_shader.set_int32("texture2", 1);

	glEnable(GL_DEPTH_TEST);

	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
}

void Renderer::render() const
{
	// wireframe on/off
	glPolygonMode(GL_FRONT_AND_BACK, m_is_wireframe_active ? GL_LINE : GL_FILL);

	// clear the screen if not drawing in full to avoid flickering
	// clear the depth buffer from the previous frame
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	m_shader.use();

	glm::mat4 view = { 1.0f };
	// note that we're translating the scene in the reverse direction of where we want to move
	view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));

	glm::mat4 projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);
	
	m_shader.set_mat4("view", view);
	m_shader.set_mat4("projection", projection);

	glActiveTexture(GL_TEXTURE0);  // activate the texture unit first before binding texture
	glBindTexture(GL_TEXTURE_2D, m_texture);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, m_texture2);

	glBindVertexArray(m_vao);
	// since we aren't using indices...
	// glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

	for(unsigned int i = 0; i < 10; i++)
	{
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, CUBE_POSITIONS[i]);
		float angle = 20.0f * i; 
		model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
		m_shader.set_mat4("model", model);

		glDrawArrays(GL_TRIANGLES, 0, 36);
	}
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
			if (reset()) {  // NOLINT(*-branch-clone)
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

b8 Renderer::reset()
{
	m_is_shader_reloading = true;
	m_shader = { CoreShaderFile("vertex_shader.vert"), CoreShaderFile("fragment_shader.frag") };
	m_is_shader_reloading = false;

	setup_rendering();

	return m_shader.is_valid();
}
}  // namespace core
