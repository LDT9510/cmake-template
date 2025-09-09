#pragma once

#include "core/shader.h"
#include "core/types.h"

namespace core
{
class EventHandler;
class Filesystem;

class Renderer {
public:
	Renderer();
	~Renderer();

	Renderer(const Renderer& other) = delete;
	Renderer& operator=(const Renderer& other) = delete;
	Renderer(Renderer&& other) noexcept = default;
	Renderer& operator=(Renderer&& other) noexcept = default;

	void setup_rendering() const;
	void render() const;
	void handle_input(const EventHandler& event_handler);
	void prepare_dev_ui();
	b8   reload_shaders();

private:
	Shader m_shader;
	u32    m_vao{};
	u32    m_vao2{};
	u32    m_vbo{};
	u32    m_vbo2{};
	u32    m_ebo{};
	u32    m_texture{};
	b8     m_is_shader_reloading{};
	b8     m_is_wireframe_active{};
};
}  // namespace core
