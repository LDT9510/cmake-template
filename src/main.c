#include <stdio.h>

int main(void)
{
	printf("Hello C World\n");

	printf("Build type is: ");

#ifdef DEV_BUILD
	printf("Debug\n");
#elif defined(SHIPPING_BUILD)
	printf("Release (Shipping)\n");
#elif defined(PROFILE_BUILD)
	printf("Profile\n");
#else
	printf("Unknown\n");
#endif
	
	printf("Compiler is: ");

#ifdef COMPILER_CLANG
	printf("Clang\n");
#elif defined(COMPILER_GCC)
	printf("GCC\n");
#elif defined(COMPILER_MSVC)
	printf("Visual Studio\n");
#else
	printf("Unknown\n");
#endif
	
	printf("Compiler front-end is: ");

#ifdef COMPILER_GNU_LIKE
	printf("GCC-like\n");
#elif defined(COMPILER_MSVC_LIKE)
	printf("MSVC-like\n");
#else
	printf("Unknown");
#endif

	printf("Target platform is: ");

#ifdef PLATFORM_WINDOWS
	printf("Windows\n");
#elif defined(PLATFORM_LINUX)
	printf("Linux\n");
#elif defined(PLATFORM_MACOS)
	printf("MacOS\n");
#elif defined(PLATFORM_WEB)
	printf("Web\n");
#else
	printf("Unknown");
#endif

	return 0;
}
