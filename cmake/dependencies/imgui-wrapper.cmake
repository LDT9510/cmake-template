include("Utils")
include("ExternalsUtils")

block()
    set(LIB_NAME "imgui-wrapper")
    announce("Configuring \"${LIB_NAME}\" external library.")
    
    force_bool(IMWRAPPER_DEMO ON)
    force_bool(IMWRAPPER_SDL3_BACKEND ON)
    force_bool(IMWRAPPER_OPENGL3_BACKEND ON)
    force_bool(IMWRAPPER_DISABLE_DEFAULT_FONT ON)
    force_bool(IMWRAPPER_EXAMPLES OFF)
    force_bool(IMWRAPPER_C_BINDINGS OFF)

    add_external_subdirectory(${LIB_NAME})
    
    if (PLATFORM_WINDOWS AND COMPILER_CLANG)
        # fix CLang on Windows
        target_compile_definitions(imgui_wrapper PUBLIC "_CRT_USE_BUILTIN_OFFSETOF")
    endif ()
endblock()
