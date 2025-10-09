#include "dev_ui.h"

#include <SDL3/SDL.h>
#include <imgui/backend/imgui_impl_opengl3.h>
#include <imgui/backend/imgui_impl_sdl3.h>
#include <dev_ui/font_roboto_medium.h>
#include <tracy/Tracy.hpp>

static constexpr auto YELLOW = ImVec4(1.0f, 1.0f, 0.0f, 1.0f);

#define CREATE_SHORTCUT(name, description) \
	do                                     \
	{                                      \
		ImGui::TextColored(YELLOW, name);  \
		ImGui::TableNextColumn();          \
		ImGui::Text(description);          \
	} while (0)

namespace dev_ui
{
	void init_for_window(SDL_Window* sdl_window, SDL_GLContext sdl_context)
	{
		ImGui::CreateContext();
		// second parameter is not used
		ImGui_ImplSDL3_InitForOpenGL(sdl_window, sdl_context);
		ImGui_ImplOpenGL3_Init();

		ImGui::GetIO().Fonts->AddFontFromMemoryCompressedTTF(
		        &g_roboto_medium_compressed_data, ROBOTO_MEDIUM_COMPRESSED_SIZE, 20.0f
		);
	}

	void create_frame()
	{
		ZoneScopedN("DevUI start");
		ImGui_ImplSDL3_NewFrame();
		ImGui_ImplOpenGL3_NewFrame();
		ImGui::NewFrame();
	}

	void render_frame()
	{
		ZoneScopedN("DevUI render");
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}

	void shutdown()
	{
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplSDL3_Shutdown();
		ImGui::DestroyContext();
	}

	void process_input(const SDL_Event& event)
	{
		ImGui_ImplSDL3_ProcessEvent(&event);
	}

	void prepare_shortcuts_ui()
	{
		if (ImGui::CollapsingHeader("Global Shortcuts"))
		{
			constexpr ImGuiTableFlags table_flags = ImGuiTableFlags_RowBg | ImGuiTableFlags_Borders;
			if (ImGui::BeginTable("shortcuts", 2, table_flags))
			{
				ImGui::TableSetupColumn("Shortcut", ImGuiTableColumnFlags_WidthFixed);
				ImGui::TableSetupColumn("Description", ImGuiTableColumnFlags_WidthFixed);
				ImGui::TableHeadersRow();
				ImGui::TableNextRow();
				ImGui::TableNextColumn();
				ImGui::Indent();
				{
					CREATE_SHORTCUT("U", "Enables wireframe mode");
					ImGui::TableNextColumn();
					CREATE_SHORTCUT("I", "Toggle UI and capture mouse");
				}
				ImGui::EndTable();
			}
		}
	}
}  // namespace dev_ui
