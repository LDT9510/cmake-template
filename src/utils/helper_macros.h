#pragma once

/** Helper macro, mainly to be used by emtpy macros to force the caller to put
 *  a semicolon when empty.
 */
#define NOOP ((void)0)

#ifdef _MSC_VER
#	include <intrin.h>
/** If a debugger is attached it will stop execution here,
        if no debugger is present the process will likely crash. */
#	define DEBUG_BREAK (__nop(), __debugbreak())
#elif defined(__clang__) || defined(__GNUC__)
/** If a debugger is attached it will stop execution here,
        if no debugger is present the process will likely crash. */
#	define DEBUG_BREAK() __builtin_debugtrap()
#else
// Fallback
#	include <signal.h>
/** If a debugger is attached it will stop execution here,
        if no debugger is present the process will likely crash. */
#	define DEBUG_BREAK() raise(SIGTRAP)
#endif

/** Shorthand notation to the attribute <c>[[maybe_unused]]</c>. */
#define M_UNUSED [[maybe_unused]]
