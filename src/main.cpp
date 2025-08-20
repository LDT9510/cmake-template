#include <fmt/format.h>

int main()
{
	fmt::println("Hello C++ World");

	fmt::print("Build type is: ");

#ifdef DEV_BUILD
	fmt::println("Debug");
#elif defined(SHIPPING_BUILD)
	fmt::println("Release (Shipping)");
#elif defined(PROFILE_BUILD)
	fmt::println("Profile");
#else
	fmt::println("Unknown");
#endif
	
	fmt::print("Compiler is: ");

#ifdef COMPILER_CLANG
	fmt::println("Clang");
#elif defined(COMPILER_GCC)
	fmt::println("GCC");
#elif defined(COMPILER_MSVC)
	fmt::println("Visual Studio");
#else
	fmt::println("Unknown");
#endif
	
	fmt::print("Compiler front-end is: ");

#ifdef COMPILER_GNU_LIKE
	fmt::println("GCC-like");
#elif defined(COMPILER_MSVC_LIKE)
	fmt::println("MSVC-like");
#else
	fmt::println("Unknown");
#endif

	fmt::print("Target platform is: ");

#ifdef PLATFORM_WINDOWS
	fmt::println("Windows");
#elif defined(PLATFORM_LINUX)
	fmt::println("Linux");
#elif defined(PLATFORM_MACOS)
	fmt::println("MacOS");
#elif defined(PLATFORM_WEB)
	fmt::println("Web");
#else
	fmt::println("Unknown");
#endif

	return 0;
}