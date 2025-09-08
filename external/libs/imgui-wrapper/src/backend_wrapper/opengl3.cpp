// do not include the backend wrapper header here, as it includes "dcimgui.h"
// and conflicts with "imgui.h" from the original backend

#include "imgui_impl_opengl3.h"

extern "C" {
  bool ImGui_C_ImplOpenGL3_Init(const char *glsl_version) {
    return ImGui_ImplOpenGL3_Init(glsl_version);
  }

  void ImGui_C_ImplOpenGL3_Shutdown() { ImGui_ImplOpenGL3_Shutdown(); }

  void ImGui_C_ImplOpenGL3_NewFrame() { ImGui_ImplOpenGL3_NewFrame(); }

  void ImGui_C_ImplOpenGL3_RenderDrawData(ImDrawData *draw_data) {
    ImGui_ImplOpenGL3_RenderDrawData(draw_data);
  }

  bool ImGui_C_ImplOpenGL3_CreateDeviceObjects() {
    return ImGui_ImplOpenGL3_CreateDeviceObjects();
  }

  void ImGui_C_ImplOpenGL3_DestroyDeviceObjects() {
    ImGui_ImplOpenGL3_DestroyDeviceObjects();
  }

  void ImGui_C_ImplOpenGL3_UpdateTexture(ImTextureData *tex) {
    ImGui_ImplOpenGL3_UpdateTexture(tex);
  }
}
