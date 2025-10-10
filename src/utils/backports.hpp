#pragma once

namespace core::backports
{

	/**
	 * Uses compiler specific extensions if possible. Even if no extension is used,
	 * undefined behavior is still raised by an empty function body and the noreturn
	 * attribute. Backport of https://en.cppreference.com/w/cpp/utility/unreachable
	 * (C++23, uses the suggested implementation from the page).
	 */
	[[noreturn]] inline void unreachable()
	{
#if defined(_MSC_VER)  // MSVC
		__assume(false);
#else  // GCC, Clang
		__builtin_unreachable();
#endif
	}

}  // namespace core::backports
