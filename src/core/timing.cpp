#include "timing.h"

#include <SDL3/SDL.h>

namespace core
{
f32 timing::get_sdl_elapsed_seconds()
{
	return static_cast<f32>(SDL_GetTicks()) / 1000.f;
}
}  // namespace core
