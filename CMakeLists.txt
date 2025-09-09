cmake_minimum_required(VERSION 3.23)
project("opengl-learn-cpp" CXX)

# set the CMake modules path, required to import using " include(...)"
list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")

# ========== Global Configuration ===========
include("Utils")
include("GlobalConfig")
include("Options")
include("SymlinkContent")
include("ExternalsUtils")
include("Definitions")

# ========== Dependencies ===========
# order matters for transitive dependencies!
include("dependencies/SDL3")
include("dependencies/fmt")
include("dependencies/glad")
include("dependencies/spdlog")
include("dependencies/imgui-wrapper")
include("dependencies/glm")
include("dependencies/physfs")
include("dependencies/stb_image")
include("dependencies/tracy")

# ============================ Main Project =============================
announce("Configuring main project")

# project sources
set(project_files
        src/main.cpp
        src/core/event_handler.cpp
        src/core/filesystem.cpp
        src/core/renderer.cpp
        src/core/shader.cpp
        src/core/timing.cpp
        src/core/window.cpp
        src/dev_ui/dev_ui.cpp
        src/utils/texture_utils.cpp)

# main executable, includes, linked libraries and compiler flags
add_executable(${PROJECT_NAME} ${project_files})
target_include_directories(${PROJECT_NAME} PRIVATE "src")
target_link_libraries(${PROJECT_NAME} PRIVATE
        SDL3::SDL3
        fmt::fmt
        spdlog::spdlog
        glad
        glm::glm
        imgui_wrapper
        PhysFS::PhysFS-static
        stb_image
        Tracy::TracyClient
)

# compiler flags
if (COMPILER_GNU_LIKE)
    target_compile_options(${PROJECT_NAME} PRIVATE
            -Wall
            -Wextra
            -W
            -Werror
            -pedantic-errors
            -Wno-language-extension-token
            -std=c++20
    )
elseif (COMPILER_MSVC_LIKE)
    target_compile_options(${PROJECT_NAME} PRIVATE
            /W4
            /WX
            /std:c++20
    )
else ()
    message(FATAL_ERROR "Unknown compiler frontend.")
endif ()

if (NOT PLATFORM_WEB)
    # content handling
    symlink_content(${PROJECT_NAME} "contents")
else ()
    # handle content when targeting the web, this is different 
    # from other platforms
endif ()

# source code macro pre-definitions
setup_target_compiler_definitions(${PROJECT_NAME})

# platform
message(STATUS "Building for '${CMAKE_SYSTEM_NAME}' platform")
