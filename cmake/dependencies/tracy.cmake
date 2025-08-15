include("Utils")
include("ExternalsUtils")

block()
    set(LIB_NAME "tracy")
    announce("Configuring \"${LIB_NAME}\" profiler.")
    
    # building options
    force_bool(TRACY_STATIC ON)  # Whether to build Tracy as a static library
    force_bool(TRACY_Fortran OFF)  # Build Fortran bindings
    force_bool(TRACY_LTO OFF)  # Enable Link-Time optimization

    # this key is also overridable, but is better to use the build type to 
    # set it, can be forced for other build types
    if(CONFIG_IS_PROFILE OR ENABLE_TRACY_PROFILING)
        force_bool(TRACY_ENABLE ON)  # Enable profiling
    else ()
        force_bool(TRACY_ENABLE OFF)  # Disable profiling
    endif ()
    
    # ======= default profiler configuration =======
    # Note: all these keys are overridable with definitions of the same name
    force_bool(TRACY_ON_DEMAND OFF)  # On-demand profiling
    force_bool(TRACY_CALLSTACK OFF)  # Enforce callstack collection for tracy regions
    force_bool(TRACY_NO_CALLSTACK OFF)  # Disable all callstack related functionality
    force_bool(TRACY_NO_CALLSTACK_INLINES OFF)  # Disables the inline functions in callstacks
    force_bool(TRACY_ONLY_LOCALHOST OFF)  # Only listen on the localhost interface
    force_bool(TRACY_NO_BROADCAST OFF)  # Disable client discovery by broadcast to local network
    force_bool(TRACY_ONLY_IPV4 OFF)  # Tracy will only accept connections on IPv4 addresses (disable IPv6)
    force_bool(TRACY_NO_CODE_TRANSFER OFF)  # Disable collection of source code
    force_bool(TRACY_NO_CONTEXT_SWITCH OFF)  # Disable capture of context switches
    force_bool(TRACY_NO_EXIT OFF)  # Client executable does not exit until all profile data is sent to server
    force_bool(TRACY_NO_SAMPLING OFF)  # Disable call stack sampling
    force_bool(TRACY_NO_VERIFY OFF)  # Disable zone validation for C API
    force_bool(TRACY_NO_VSYNC_CAPTURE OFF)  # Disable capture of hardware Vsync events
    force_bool(TRACY_NO_FRAME_IMAGE OFF)  # Disable the frame image support and its thread
    force_bool(TRACY_NO_SYSTEM_TRACING OFF)  # Disable systrace sampling
    force_bool(TRACY_PATCHABLE_NOPSLEDS OFF)  # Enable nopsleds for efficient patching by system-level tools (e.g. rr)
    force_bool(TRACY_DELAYED_INIT OFF)  # Enable delayed initialization of the library (init on first call)
    force_bool(TRACY_MANUAL_LIFETIME OFF)  # Enable the manual lifetime management of the profile
    force_bool(TRACY_FIBERS OFF)  # Enable fibers support
    force_bool(TRACY_NO_CRASH_HANDLER OFF)  # Disable crash handling
    force_bool(TRACY_TIMER_FALLBACK OFF)  # Use lower resolution timers
    force_bool(TRACY_LIBUNWIND_BACKTRACE OFF)  # Use libunwind backtracing where supported
    force_bool(TRACY_SYMBOL_OFFLINE_RESOLVE OFF)  # Instead of full runtime symbol resolution, only resolve the image path and offset to enable offline symbol resolution
    force_bool(TRACY_LIBBACKTRACE_ELF_DYNLOAD_SUPPORT OFF)  # Enable libbacktrace to support dynamically loaded elfs in symbol resolution resolution after the first symbol resolve operation
    force_bool(TRACY_DEBUGINFOD OFF)  # Enable debuginfod support

    add_external_subdirectory(${LIB_NAME})
endblock()
