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
{
}

Renderer::~Renderer()
{
}

void Renderer::setup_rendering() const
{
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
}

void Renderer::render() const
{
	glClear(GL_COLOR_BUFFER_BIT);
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
