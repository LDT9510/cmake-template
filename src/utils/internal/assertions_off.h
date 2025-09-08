#pragma once

#include "utils/backports.h"
#include "utils/helper_macros.h"

#define check_msg(expr, msg) NOOP

#define check(expr) NOOP

#define check_unreachable() core::backports::unreachable()

#define verify_msg(expr, msg) expr

#define verify(expr) expr
