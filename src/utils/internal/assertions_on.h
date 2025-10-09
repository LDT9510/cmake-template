#pragma once

#include "utils/helper_macros.h"

#include <spdlog/spdlog.h>

#define CHECK_MSG(expr, msg)                             \
	do                                                   \
	{                                                    \
		if (!(expr))                                     \
		{                                                \
			SPDLOG_ERROR("Assertion failed: '{}'", msg); \
			DEBUG_BREAK;                                 \
		}                                                \
	} while (0)

#define CHECK(expr) check_msg(expr, #expr)

#define CHECK_UNREACHABLE() check_msg(false, "unreachable code reached")

#define VERIFY_MSG(expr, msg) check_msg(expr, msg)

#define VERIFY(expr) check(expr)
