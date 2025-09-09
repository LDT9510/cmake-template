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
	glGenBuffers(1, &m_vbo);
	glGenBuffers(1, &m_ebo);
}

Renderer::~Renderer()
{
	glDeleteVertexArrays(1, &m_vao);
	glDeleteBuffers(1, &m_vbo);
	glDeleteBuffers(1, &m_ebo);
}

void Renderer::setup_rendering() const
{
	// clang-format off
	static constexpr std::array VERTICES = {
		 0.5f,  0.5f, 0.0f,  // top right
		 0.5f, -0.5f, 0.0f,  // bottom right
		-0.5f, -0.5f, 0.0f,  // bottom left
		-0.5f,  0.5f, 0.0f   // top left 
   };

	static constexpr std::array INDICES = {  // note that we start from 0!
		0, 1, 3,   // first triangle
		1, 2, 3    // second triangle
	};
	// clang-format on
	// 0. bind Vertex Array Object and copy our vertices array in a buffer for OpenGL to use
	glBindVertexArray(m_vao);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(VERTICES), VERTICES.data(), GL_STATIC_DRAW);
	// 1. then set the vertex attributes pointers
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
	glEnableVertexAttribArray(0);
	// 2. copy our index array in a element buffer for OpenGL to use
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(INDICES), INDICES.data(), GL_STATIC_DRAW);
	// 3. use our shader program when we want to render an object
	m_shader.use();
	// 4. now draw the object  in `Renderer::render()`

	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
}

void Renderer::render() const
{
	// clear the screen if not drawing in full to avoid flickering
	glClear(GL_COLOR_BUFFER_BIT);

	m_shader.use();
	glBindVertexArray(m_vao);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void Renderer::prepare_dev_ui()
{
	ImGui::Begin("Learning OpenGL");
	ImGui::End();
}

void Renderer::handle_input(const EventHandler& event_handler)
{
	(void)event_handler;
}

b8 Renderer::reload_shaders()
{
	return false;
}
}  // namespace core
