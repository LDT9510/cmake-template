#pragma once

#include "utils/helper_macros.h"

#include <spdlog/spdlog.h>

#define check_msg(expr, msg)                             \
	do {                                                 \
		if (!(expr)) {                                   \
			SPDLOG_ERROR("Assertion failed: '{}'", msg); \
			DEBUG_BREAK;                                 \
		}                                                \
	} while (0)

#define check(expr) check_msg(expr, #expr)

#define check_unreachable() check_msg(false, "unreachable code reached")

#define verify_msg(expr, msg) check_msg(expr, msg)

#define verify(expr) check(expr)
