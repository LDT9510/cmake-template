include("Utils")
include("ExternalsUtils")

block()
    set(LIB_NAME "SDL3")
    announce("Configuring \"${LIB_NAME}\" external library.")

    # ====================== options ======================
    force_bool(SDL_SHARED ON)  # Build a shared version of the library
    force_bool(SDL_STATIC OFF)  # Build a static version of the library

    # for some platforms, always build static
    if (TARGET_PLATFORM_IS_WEB)
        force_bool(SDL_SHARED OFF)
        force_bool(SDL_STATIC ON)
    endif ()

    force_bool(SDL_TESTS OFF)  # Build the test directory
    force_bool(SDL_TEST_LIBRARY OFF)  # Build the SDL3_test library
    force_bool(SDL_INSTALL OFF)  # Enable installation of SDL3
    force_bool(SDL_UNINSTALL OFF)  # Enable uninstallation of SDL3
    force_bool(SDL_EXAMPLES OFF)  # Build the examples directory
    force_bool(SDL_INSTALL_TESTS OFF)  # Install tests target
    force_bool(SDL_DISABLE_INSTALL_DOCS ON)  # Disable Install docs target

    # =================== subsystems ======================
    force_bool(SDL_VIDEO ON)  # Enable the Video subsystem
    force_bool(SDL_GPU OFF)  # Enable the GPU subsystem
    force_bool(SDL_CAMERA OFF)  # Enable the Camera subsystem
    force_bool(SDL_RENDER OFF)  # Enable the Render subsystem
    force_bool(SDL_JOYSTICK ON)  # Enable the Joystick subsystem
    force_bool(SDL_SENSOR OFF)  # Enable the Sensor subsystem
    force_bool(SDL_HAPTIC OFF)  # Enable the Haptic subsystem
    force_bool(SDL_HIDAPI ON)  # Enable the HIDAPI subsystem
    force_bool(SDL_AUDIO OFF)  # Enable the Audio subsystem
    force_bool(SDL_POWER OFF)  # Enable the Power subsystem

    # ==================== rendering and graphics APIs =======================
    force_bool(SDL_DIRECTX OFF)  # Use DirectX for Windows audio/video
    force_bool(SDL_OPENGL ON)  # Include OpenGL support
    force_bool(SDL_OPENGLES OFF)  # Include OpenGL ES support
    force_bool(SDL_VULKAN OFF)  # Enable Vulkan support
    force_bool(SDL_OPENVR OFF)  # Use OpenVR video driver
    force_bool(SDL_RENDER_D3D OFF)  # Enable the Direct3D 9 render driver
    force_bool(SDL_RENDER_D3D11 OFF)  # Enable the Direct3D 11 render driver
    force_bool(SDL_RENDER_D3D12 OFF)  # Enable the Direct3D 12 render driver
    force_bool(SDL_RENDER_GPU OFF)  # Enable the SDL_GPU render driver
    force_bool(SDL_RENDER_METAL OFF)  # Enable the Metal render driver
    force_bool(SDL_RENDER_VULKAN OFF)  # Enable the Vulkan render driver

    #    # ======= advanced configurations (these should be left to auto-detection)  =======
    #    force_bool(SDL_ALSA OFF)  # Support the ALSA audio API
    #    force_bool(SDL_ASAN OFF)  # Use AddressSanitizer to detect memory errors
    #    force_bool(SDL_ASSEMBLY ON)  # Enable assembly routines
    #    force_string(SDL_ASSERTIONS "auto")  # Enable internal sanity checks (auto/disabled/release/enabled/paranoid)
    #    force_bool(SDL_AVX ON)  # Use AVX assembly routines
    #    force_bool(SDL_AVX2 ON)  # Use AVX2 assembly routines
    #    force_bool(SDL_AVX512F ON)  # Use AVX512F assembly routines
    #    force_string(SDL_BACKGROUNDING_SIGNAL OFF)  # number to use for magic backgrounding signal or 'OFF'
    #    force_bool(SDL_CCACHE ON)  # Use Ccache to speed up build
    #    force_bool(SDL_CLANG_TIDY OFF)  # Run clang-tidy static analysis
    #    force_bool(SDL_CLOCK_GETTIME OFF)  # Use clock_gettime() instead of gettimeofday()
    #    force_bool(SDL_DEPS_SHARED ON)  # Load dependencies dynamically
    #    force_bool(SDL_DIALOG ON)  # Enable the Dialog subsystem
    #    force_bool(SDL_DISKAUDIO ON)  # Support the disk writer audio driver
    #    force_bool(SDL_DUMMYAUDIO OFF)  # Support the dummy audio driver
    #    force_bool(SDL_DUMMYCAMERA OFF)  # Support the dummy camera driver
    #    force_bool(SDL_DUMMYVIDEO OFF)  # Use dummy video driver
    #    force_string(SDL_FOREGROUNDING_SIGNAL OFF)  # number to use for magic foregrounding signal or 'OFF'
    #    force_bool(SDL_GCC_ATOMICS OFF)  # Use gcc builtin atomics
    #    force_bool(SDL_GPU_DXVK OFF)  # Build SDL_GPU with DXVK support
    #    force_bool(SDL_HIDAPI_JOYSTICK ON)  # Use HIDAPI for low level joystick drivers
    #    force_bool(SDL_HIDAPI_LIBUSB ON)  # Use libusb for low level joystick drivers
    #    force_bool(SDL_HIDAPI_LIBUSB_SHARED ON)  # Dynamically load libusb support
    #    force_string(SDL_INSTALL_CMAKEDIR_ROOT cmake)  # Root folder where to install SDL3Config.cmake related files (SDL3 subfolder for MSVC projects)
    #    force_bool(SDL_JACK OFF)  # Support the JACK audio API
    #    force_bool(SDL_KMSDRM OFF)  # Use KMS DRM video driver
    #    force_bool(SDL_LIBC ON)  # Use the system C library
    #    force_bool(SDL_LIBICONV OFF)  # Prefer iconv() from libiconv, if available, over libc version
    #    force_bool(SDL_LIBUDEV ON)  # Enable libudev support
    #    force_bool(SDL_MMX ON)  # Use MMX assembly routines
    #    force_bool(SDL_OFFSCREEN OFF)  # Use offscreen video driver
    #    force_bool(SDL_PIPEWIRE OFF)  # Use Pipewire audio
    #    force_bool(SDL_PRESEED ON)  # Preseed CMake cache to speed up configuration
    #    force_bool(SDL_PTHREADS OFF)  # Use POSIX threads for multi-threading
    #    force_bool(SDL_PULSEAUDIO OFF)  # Use PulseAudio
    #    force_bool(SDL_RPATH OFF)  # Use an rpath when linking SDL
    #    force_bool(SDL_SNDIO OFF)  # Support the sndio audio API
    #    force_bool(SDL_SSE ON)  # Use SSE assembly routines
    #    force_bool(SDL_SSE2 ON)  # Use SSE2 assembly routines
    #    force_bool(SDL_SSE3 ON)  # Use SSE3 assembly routines
    #    force_bool(SDL_SSE4_1 ON)  # Use SSE4.1 assembly routines
    #    force_bool(SDL_SSE4_2 ON)  # Use SSE4.2 assembly routines
    #    force_bool(SDL_SYSTEM_ICONV OFF)  # Use iconv() from system-installed libraries
    #    force_bool(SDL_WASAPI ON)  # Use the Windows WASAPI audio driver
    #    force_bool(SDL_WAYLAND OFF)  # Use Wayland video driver
    #    force_bool(SDL_WERROR OFF)  # Enable -Werror
    #    sforce_bool(SDL_X11 OFF)  # Use X11 video driver
    #    force_bool(SDL_XINPUT ON)  # Use Xinput for Windows
    #    force_bool(SDL_VIRTUAL_JOYSTICK ON)  # Enable the virtual-joystick driver
    #    force_bool(SDL_XINPUT ON)  # Use Xinput for Windows

    add_external_subdirectory(${LIB_NAME})
endblock()