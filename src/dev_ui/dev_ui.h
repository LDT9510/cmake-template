#pragma once

// ReSharper disable CppInconsistentNaming
union SDL_Event;
struct SDL_Window;
struct SDL_GLContextState;
// ReSharper restore CppInconsistentNaming

namespace dev_ui
{

void init_for_window(SDL_Window* sdl_window, SDL_GLContextState* sdl_context);
void create_frame();
void render_frame();
void shutdown();
void process_input(const SDL_Event& event);

}  // namespace dev_ui
