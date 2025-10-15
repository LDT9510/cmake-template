cmake_minimum_required(VERSION 3.23)
project(imgui_wrapper
        VERSION 1.92.2
        DESCRIPTION "Custom Dear ImGui wrapper using CMake, for C and C++."
        LANGUAGES C CXX
)

# compilation is done as a single static library
option(IMWRAPPER_EXAMPLES "Compile example projects" ON)
option(IMWRAPPER_DEMO "Compile the ImGui demo project (this is the regular C++ project)" ON)
option(IMWRAPPER_C_BINDINGS "Compile the C bindings, for the library and the backend" ON)
option(IMWRAPPER_DISABLE_DEFAULT_FONT "Disable the default font, user should provide one or crash" OFF)

# NOTE: user code is responsible for providing externally linked libraries required by the backends

# Backend Platforms
if (WIN32)
    option(IMWRAPPER_WIN32_BACKEND "Compile the ImGui Windows platform backend" OFF)
endif ()
option(IMWRAPPER_SDL3_BACKEND "Compile the ImGui SDL3 platform backend" OFF)

# Backend Renderers
option(IMWRAPPER_OPENGL3_BACKEND "Compile the ImGui OpenGL 3 renderer backend" OFF)

set(SOURCES_DIR src)

set(IMGUI_SOURCES
        # ========== Dear ImGui ==========
        "${SOURCES_DIR}/imgui.cpp"
        "${SOURCES_DIR}/imgui_draw.cpp"
        "${SOURCES_DIR}/imgui_tables.cpp"
        "${SOURCES_DIR}/imgui_widgets.cpp"
        $<$<BOOL:${IMWRAPPER_DEMO}>:${SOURCES_DIR}/imgui_demo.cpp>)

set(IMGUI_BACKEND_SOURCES
        # ========== original sources ==========
        $<$<BOOL:${IMWRAPPER_WIN32_BACKEND}>:${SOURCES_DIR}/original_backend/imgui_impl_win32.cpp>
        $<$<BOOL:${IMWRAPPER_SDL3_BACKEND}>:${SOURCES_DIR}/original_backend/imgui_impl_sdl3.cpp>
        $<$<BOOL:${IMWRAPPER_OPENGL3_BACKEND}>:${SOURCES_DIR}/original_backend/imgui_impl_opengl3.cpp>)

if (IMWRAPPER_C_BINDINGS)
    set(IMWRAPPER_BINDING_SOURCES
            # ========== Dear Bindings ==========
            "${SOURCES_DIR}/dcimgui.cpp"
            "${SOURCES_DIR}/dcimgui_internal.cpp"
            # ========== backend binding sources ==========
            $<$<BOOL:${IMWRAPPER_WIN32_BACKEND}>:${SOURCES_DIR}/c_backend_wrapper/win32.cpp>
            $<$<BOOL:${IMWRAPPER_SDL3_BACKEND}>:${SOURCES_DIR}/c_backend_wrapper/sdl3.cpp>
            $<$<BOOL:${IMWRAPPER_OPENGL3_BACKEND}>:${SOURCES_DIR}/c_backend_wrapper/opengl3.cpp>
    )
endif ()

add_library(${PROJECT_NAME} STATIC ${IMGUI_SOURCES} ${IMGUI_BACKEND_SOURCES} ${IMWRAPPER_BINDING_SOURCES})

target_include_directories(${PROJECT_NAME}
        PUBLIC
        "include"
        PRIVATE
        "include/imgui_c" "include/imgui" "include/imgui/backend" ${SOURCES_DIR})

target_compile_definitions(${PROJECT_NAME} PUBLIC
        # disable obsolete API
        IMGUI_DISABLE_OBSOLETE_FUNCTIONS
        IMGUI_DISABLE_OBSOLETE_KEYIO

        # makes excluding te default font easier
        $<$<BOOL:${IMWRAPPER_DISABLE_DEFAULT_FONT}>:IMGUI_DISABLE_DEFAULT_FONT>)

# ==== required libraries declare linking here, but should be provided by the user code ====
if (IMWRAPPER_SDL3_BACKEND)
    target_link_libraries(${PROJECT_NAME} PRIVATE SDL3::SDL3)
endif ()

# add the examples
if ("${IMWRAPPER_EXAMPLES}")
    if (WIN32)
        add_subdirectory(examples/win32_opengl3)
    endif ()

    add_subdirectory(examples/sdl3_opengl3)
endif ()

message(STATUS "${PROJECT_NAME} configured correctly")