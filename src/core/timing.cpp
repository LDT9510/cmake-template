#include "timing.h"

#include <SDL3/SDL.h>

namespace core
{
namespace
{
static f32 g_delta_time = 0.0f;
static f32 g_last_frame_time = 0.0f;
}

f32 timing::get_elapsed_seconds()
{
	return (f32)SDL_GetTicks() / 1000.f;
}

f32 timing::get_delta_time()
{
	return g_delta_time;
}

void timing::update_delta_time()
{
	f32 current_frame_time = get_elapsed_seconds();
	g_delta_time = current_frame_time - g_last_frame_time;
	g_last_frame_time = current_frame_time; 
}

}  // namespace core
