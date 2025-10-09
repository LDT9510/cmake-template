#include "core/renderer.h"

#include "core/camera.h"
#include "core/event_handler.h"
#include "core/filesystem.h"
#include "core/timing.h"
#include "core/window.h"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "utils/texture_utils.h"

#include <span>
#include <glad/gl.h>
#include <imgui/imgui.h>
#include <spdlog/spdlog.h>
#include <stb/stb_image.h>
#include <glm/glm.hpp>
#include <tracy/Tracy.hpp>
#include <tracy/TracyOpenGL.hpp>

namespace
{
	using namespace core;
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

	static constexpr std::array CUBE_POSITIONS = { glm::vec3(0.0f, 0.0f, 0.0f),    glm::vec3(2.0f, 5.0f, -15.0f),
		                                           glm::vec3(-1.5f, -2.2f, -2.5f), glm::vec3(-3.8f, -2.0f, -12.3f),
		                                           glm::vec3(2.4f, -0.4f, -3.5f),  glm::vec3(-1.7f, 3.0f, -7.5f),
		                                           glm::vec3(1.3f, -2.0f, -2.5f),  glm::vec3(1.5f, 2.0f, -2.5f),
		                                           glm::vec3(1.5f, 0.2f, -1.5f),   glm::vec3(-1.3f, 1.0f, -1.5f) };

	static f32 g_aspect_ratio = 16.0f / 9.0f;
}  // namespace

core::Renderer::Renderer(const core::Window& window, const core::Camera& camera)
    : m_shader{ CoreShaderFile("vertex_shader.vert"), CoreShaderFile("fragment_shader.frag") }
    , m_window{ &window }
    , m_camera{ &camera }
{
	glGenVertexArrays(1, &m_vao);
	glGenBuffers(1, &m_vbo);
	glGenBuffers(1, &m_ebo);
	glGenTextures(1, &m_texture);
	glGenTextures(1, &m_texture2);

	g_aspect_ratio = m_window->get_aspect_ratio();
}

core::Renderer::~Renderer()
{
	glDeleteVertexArrays(1, &m_vao);
	glDeleteBuffers(1, &m_vbo);
	glDeleteBuffers(1, &m_ebo);
	glDeleteTextures(1, &m_texture);
	glDeleteTextures(1, &m_texture2);
}

void core::Renderer::setup_rendering()
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
	// NOLINTNEXTLINE(*-no-int-to-ptr)
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(f32), (const void*)(3 * sizeof(f32)));
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

void core::Renderer::render() const
{
	ZoneScopedN("Render");

	{
		ZoneNamedN(RenderSetup, "RenderSetup", true);
		// wireframe on/off
		glPolygonMode(GL_FRONT_AND_BACK, m_is_wireframe_active ? GL_LINE : GL_FILL);

		// clear the screen if not drawing in full to avoid flickering
		// clear the depth buffer from the previous frame
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		{
			TracyGpuZone("Shader Setup");
			m_shader.use();

			glm::mat4 view = m_camera->get_view_matrix();

			glm::mat4 projection = glm::perspective(glm::radians(m_camera->get_zoom()), g_aspect_ratio, 0.1f, 100.0f);

			m_shader.set_mat4("view", view);
			m_shader.set_mat4("projection", projection);
		}

		glActiveTexture(GL_TEXTURE0);  // activate the texture unit first before binding texture
		glBindTexture(GL_TEXTURE_2D, m_texture);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, m_texture2);

		glBindVertexArray(m_vao);
		// since we aren't using indices...
		// glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
	}

	{
		ZoneNamedN(Draw, "Draw", true);
		for (unsigned int i = 0; i < 10; i++)
		{
			glm::mat4 model = { 1.0f };
			model = glm::translate(model, CUBE_POSITIONS[i]);
			f32 angle = 20.0f * (f32)i;

			if (i % 3 == 0)
			{
				angle = timing::get_elapsed_seconds() * 25.0f;
			}

			model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
			m_shader.set_mat4("model", model);

			{
				TracyGpuZone("Draw");
				glDrawArrays(GL_TRIANGLES, 0, 36);
			}
		}
	}
}

void core::Renderer::prepare_dev_ui()
{
	ZoneScopedN("Render prepare DevUI");

	if (ImGui::CollapsingHeader("Rendering"))
	{
		ImGui::SliderFloat("Aspect Ratio", &g_aspect_ratio, 0.1f, 2.0f);

		if (ImGui::Button("Reload shaders"))
		{
			if (!m_is_shader_reloading)
			{
				if (reset())
				{  // NOLINT(*-branch-clone)
					SPDLOG_INFO("All Shaders reloaded OK.");
				}
				else
				{
					SPDLOG_ERROR("Error reloading shaders.");
				}
			}
			else
			{
				SPDLOG_INFO("Compiling shaders, please wait...");
			}
		}
	}
}

void core::Renderer::handle_input(EventHandler& event_handler)
{
	if (event_handler.is_key_just_pressed(SDLK_U))
	{
		m_is_wireframe_active = !m_is_wireframe_active;
	}

	if (event_handler.is_key_just_pressed(SDLK_I))
	{
		event_handler.toggle_mouse_capture();
	}
}

b8 core::Renderer::reset()
{
	m_is_shader_reloading = true;
	m_shader = { CoreShaderFile("vertex_shader.vert"), CoreShaderFile("fragment_shader.frag") };
	m_is_shader_reloading = false;

	setup_rendering();

	g_aspect_ratio = m_window->get_aspect_ratio();

	return m_shader.is_valid();
}
