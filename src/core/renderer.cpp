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
	glGenBuffers(1, &m_ebo);
	glGenTextures(1, &m_texture);
}

Renderer::~Renderer()
{
	glDeleteVertexArrays(1, &m_vao);
	glDeleteBuffers(1, &m_vbo);
	glDeleteBuffers(1, &m_ebo);
	glDeleteTextures(1, &m_texture);
}

void Renderer::setup_rendering() const
{
	// clang-format off
	static constexpr std::array VERTICES = {
		// positions          // colors           // texture coords
		0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
		0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
	   -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
	   -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // top left 
   };

	static constexpr std::array INDICES = {
		0, 1, 3,   // first triangle
		1, 2, 3    // second triangle
	};
	// clang-format on

	glBindVertexArray(m_vao);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(VERTICES), VERTICES.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(INDICES), INDICES.data(), GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(f32), nullptr);
	glEnableVertexAttribArray(0);

	// color attribute
	glVertexAttribPointer(
			1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(f32),
			reinterpret_cast<const void*>(3 * sizeof(f32)));  // NOLINT(*-no-int-to-ptr)
	glEnableVertexAttribArray(1);

	// texture coordinates attribute
	glVertexAttribPointer(
			2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(f32),
			reinterpret_cast<const void*>(6 * sizeof(f32)));  // NOLINT(*-no-int-to-ptr)
	glEnableVertexAttribArray(2);

	// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex
	// attribute's bound vertex buffer object so afterward we can safely unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object
	// IS stored in the VAO; keep the EBO bound. glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	// You can unbind the VAO afterward so other VAO calls won't accidentally modify this VAO,
	// but this rarely happens. Modifying other VAOs requires a call to glBindVertexArray anyway
	// so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
	glBindVertexArray(0);

	// load texture from image
	// load and generate the texture
	int  width, height, nr_channels;
	auto image_contents = fs::instance().read_file<std::vector<u8>>(CoreTextureFile("container.jpg"));
	unsigned char* data = stbi_load_from_memory(image_contents.data(),
												static_cast<i32>(image_contents.size()), &width,
												&height, &nr_channels, 0);
	if (data != nullptr) {
		// load texture data and configure
		glBindTexture(GL_TEXTURE_2D, m_texture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
		// set the texture wrapping/filtering options (on the currently bound texture
		// object)
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		static constexpr std::array BORDER_COLOR = { 1.0f, 1.0f, 0.0f, 1.0f };
		glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, BORDER_COLOR.data());
		stbi_image_free(data);
	} else {
		SPDLOG_ERROR("Failed to load texture");
	}

	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
}

void Renderer::render() const
{
	// wireframe on/off
	glPolygonMode(GL_FRONT_AND_BACK, m_is_wireframe_active ? GL_LINE : GL_FILL);

	// clear the screen if not drawing in full to avoid flickering
	glClear(GL_COLOR_BUFFER_BIT);
	
	m_shader.use();

	glBindTexture(GL_TEXTURE_2D, m_texture);
	glBindVertexArray(m_vao);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
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
