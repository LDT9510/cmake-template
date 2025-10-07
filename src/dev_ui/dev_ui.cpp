#include "dev_ui.h"

#include <SDL3/SDL.h>
#include <imgui/backend/imgui_impl_opengl3.h>
#include <imgui/backend/imgui_impl_sdl3.h>
#include <dev_ui/font_roboto_medium.h>
#include <tracy/Tracy.hpp>

namespace dev_ui
{
void init_for_window(SDL_Window* sdl_window, SDL_GLContext sdl_context)
{
	ImGui::CreateContext();
	// second parameter is not used
	ImGui_ImplSDL3_InitForOpenGL(sdl_window, sdl_context);
	ImGui_ImplOpenGL3_Init();

	ImGui::GetIO().Fonts->AddFontFromMemoryCompressedTTF(&RobotoMedium_compressed_data,
	                                                     RobotoMedium_compressed_size, 20.0f);
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
}  // namespace dev_ui
