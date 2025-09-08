include("Utils")
include("ExternalsUtils")

# REQUIRES SDL3
block()
    set(LIB_NAME "spdlog")
    announce("Configuring \"${LIB_NAME}\" external library.")

    force_bool(SPDLOG_BUILD_ALL OFF)  # Build all artifacts
    force_bool(SPDLOG_BUILD_BENCH OFF)  # Build benchmarks (Requires https://github.com/google/benchmark.git to be installed)
    force_bool(SPDLOG_BUILD_EXAMPLE OFF)  # Build example
    force_bool(SPDLOG_BUILD_EXAMPLE_HO OFF)  # Build header only example
    force_bool(SPDLOG_BUILD_PIC OFF)  # Build position independent code (-fPIC)
    force_bool(SPDLOG_BUILD_SHARED OFF)  # Build shared library
    force_bool(SPDLOG_BUILD_TESTS OFF)  # Build tests
    force_bool(SPDLOG_BUILD_TESTS_HO OFF)  # Build tests using the header only version
    force_bool(SPDLOG_BUILD_WARNINGS OFF)  # Enable compiler warnings
    force_bool(SPDLOG_CLOCK_COARSE OFF)  # non supported option
    force_bool(SPDLOG_DISABLE_DEFAULT_LOGGER OFF)  # Disable default logger creation
    force_bool(SPDLOG_ENABLE_PCH OFF)  # Build static or shared library using precompiled header to speed up compilation time
    force_bool(SPDLOG_FMT_EXTERNAL ON)  # Use external fmt library instead of bundled
    force_bool(SPDLOG_FMT_EXTERNAL_HO OFF)  # Use external fmt header-only library instead of bundled
    force_bool(SPDLOG_FWRITE_UNLOCKED ON)  # Use the unlocked variant of fwrite. Leave this on unless your libc doesn't have it
    force_bool(SPDLOG_INSTALL OFF)  # Generate the install target
    force_bool(SPDLOG_MSVC_UTF8 ON)  # Enable/disable msvc /utf-8 flag required by fmt lib
    force_bool(SPDLOG_NO_ATOMIC_LEVELS OFF)  # prevent spdlog from using of std::atomic log levels (use only if your code never modifies log levels concurrently
    force_bool(SPDLOG_NO_EXCEPTIONS OFF)  # Compile with -fno-exceptions. Call abort() on any spdlog exceptions
    force_bool(SPDLOG_NO_THREAD_ID OFF)  # prevent spdlog from querying the thread id on each log call if thread id is not needed
    force_bool(SPDLOG_NO_TLS OFF)  # prevent spdlog from using thread local storage
    force_bool(SPDLOG_PREVENT_CHILD_FD OFF)  # Prevent from child processes to inherit log file descriptors
    force_bool(SPDLOG_SANITIZE_ADDRESS OFF)  # Enable address sanitizer in tests
    force_bool(SPDLOG_SANITIZE_THREAD OFF)  # Enable thread sanitizer in tests
    force_bool(SPDLOG_SYSTEM_INCLUDES OFF)  # Include as system headers (skip for clang-tidy).
    force_bool(SPDLOG_TIDY OFF)  # run clang-tidy
    force_bool(SPDLOG_USE_STD_FORMAT OFF)  # Use std::format instead of fmt library.
    force_bool(SPDLOG_WCHAR_CONSOLE OFF)  # Support wchar output to console
    force_bool(SPDLOG_WCHAR_FILENAMES OFF)  # Support wchar filenames
    force_bool(SPDLOG_WCHAR_SUPPORT OFF)  # Support wchar api

    add_external_subdirectory(${LIB_NAME})
endblock()
