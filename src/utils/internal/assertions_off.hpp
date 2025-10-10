#pragma once

#include "utils/backports.hpp"
#include "utils/helper_macros.hpp"

#define CHECK_MSG(expr, msg) NOOP

#define CHECK(expr) NOOP

#define CHECK_UNREACHABLE() core::backports::unreachable()

#define VERIFY_MSG(expr, msg) expr

#define VERIFY(expr) expr
