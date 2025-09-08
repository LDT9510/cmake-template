// do not include the backend wrapper header here, as it includes "dcimgui.h"
// and conflicts with "imgui.h" from the original backend

#include "imgui_impl_sdl3.h"

extern "C" {
  bool ImGui_C_ImplSDL3_InitForOpenGL(SDL_Window *window, void *sdl_gl_context) {
    return ImGui_ImplSDL3_InitForOpenGL(window, sdl_gl_context);
  }
  
  // bool     ImGui_C_ImplSDL3_InitForVulkan(SDL_Window* window){}
  // bool     ImGui_C_ImplSDL3_InitForD3D(SDL_Window* window){}
  // bool     ImGui_C_ImplSDL3_InitForMetal(SDL_Window* window){}
  // bool     ImGui_C_ImplSDL3_InitForSDLRenderer(SDL_Window* window,
  // SDL_Renderer* renderer){} bool     ImGui_C_ImplSDL3_InitForSDLGPU(SDL_Window*
  // window){} bool     ImGui_C_ImplSDL3_InitForOther(SDL_Window* window){}
  
  void ImGui_C_ImplSDL3_Shutdown() {
    ImGui_ImplSDL3_Shutdown();
  }
  
  void ImGui_C_ImplSDL3_NewFrame() {
    ImGui_ImplSDL3_NewFrame();
  }
  
  bool ImGui_C_ImplSDL3_ProcessEvent(const SDL_Event *event) {
    return ImGui_ImplSDL3_ProcessEvent(event);
  }
  
  void ImGui_C_ImplSDL3_SetGamepadMode(ImGui_ImplSDL3_GamepadMode mode,
                                       SDL_Gamepad **manual_gamepads_array,
                                       int manual_gamepads_count) {
    ImGui_ImplSDL3_SetGamepadMode(mode, manual_gamepads_array, manual_gamepads_count);
  }
}
