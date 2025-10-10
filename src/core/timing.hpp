#pragma once

#include "core/types.hpp"

namespace core::timing
{
	[[nodiscard]] f32 get_elapsed_seconds();
	[[nodiscard]] f32 get_delta_time();
	void              update_delta_time();
}  // namespace core::timing
