// do not include the backend wrapper header here, as it includes "dcimgui.h"
// and conflicts with "imgui.h" from the original backend

#include "imgui_impl_win32.h"

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

extern "C" {
  bool ImGui_C_ImplWin32_Init(void *hwnd) { return ImGui_ImplWin32_Init(hwnd); }

  bool ImGui_C_ImplWin32_InitForOpenGL(void *hwnd) {
    return ImGui_ImplWin32_InitForOpenGL(hwnd);
  }

  void ImGui_C_ImplWin32_Shutdown() { ImGui_ImplWin32_Shutdown(); }

  void ImGui_C_ImplWin32_NewFrame() { ImGui_ImplWin32_NewFrame(); }

  LRESULT ImGui_C_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    return ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam);
  }
  

  void ImGui_C_ImplWin32_EnableDpiAwareness() {
    ImGui_ImplWin32_EnableDpiAwareness();
  }

  float ImGui_C_ImplWin32_GetDpiScaleForHwnd(void *hwnd) {
    return ImGui_ImplWin32_GetDpiScaleForHwnd(hwnd);
  }

  float ImGui_C_ImplWin32_GetDpiScaleForMonitor(void *monitor) {
    return ImGui_ImplWin32_GetDpiScaleForMonitor(monitor);
  }

  void ImGui_C_ImplWin32_EnableAlphaCompositing(void *hwnd) {
    return ImGui_ImplWin32_EnableAlphaCompositing(hwnd);
  }
}