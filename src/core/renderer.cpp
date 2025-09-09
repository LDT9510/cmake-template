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
}

Renderer::~Renderer()
{
	glDeleteVertexArrays(1, &m_vao);
	glDeleteBuffers(1, &m_vbo);
}

void Renderer::setup_rendering() const
{
	// clang-format off
	static constexpr std::array VERTICES = {
		-0.5f, -0.5f, 0.0f,
		 0.5f, -0.5f, 0.0f,
		 0.0f,  0.5f, 0.0f
   };
	// clang-format on
	// 0. bind Vertex Array Object and copy our vertices array in a buffer for OpenGL to use
	glBindVertexArray(m_vao);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(VERTICES), VERTICES.data(), GL_STATIC_DRAW);
	// 1. then set the vertex attributes pointers
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// 2. use our shader program when we want to render an object
	m_shader.use();
	// 3. now draw the object  in `Renderer::render()`

	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
}

void Renderer::render() const
{
	// clear the screen if not drawing in full to avoid flickering
	glClear(GL_COLOR_BUFFER_BIT);

	m_shader.use();
	glBindVertexArray(m_vao);
	glDrawArrays(GL_TRIANGLES, 0, 3);
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
