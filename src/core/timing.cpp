#include "timing.h"

#include <SDL3/SDL.h>

namespace core
{
f32 timing::get_elapsed_seconds()
{
	return SDL_GetTicks() / 1000.f;
}
}  // namespace core
