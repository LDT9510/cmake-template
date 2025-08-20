cmake_minimum_required(VERSION 3.23)
project("cmake-template" C CXX)

# set the CMake modules path, required to import using " include(...)"
list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")

# ========== Global Configuration, options and imports ===========
include("Utils")
include("GlobalConfig")
include("Options")
include("SymlinkContent")
include("ExternalsUtils")
include("Definitions")

# ======================= Main Projects =======================
function (setup EXECUTABLE)
    announce("Configuring '${EXECUTABLE}' project")

    # compiler flags
    if (COMPILER_GNU_LIKE)
        target_compile_options(${EXECUTABLE} PRIVATE
                -Wall
                -Wextra
                -W
                -Werror
                -pedantic-errors
                -Wno-language-extension-token
        )
    elseif (COMPILER_MSVC_LIKE)
        target_compile_options(${EXECUTABLE} PRIVATE
                /W3
                /WX
        )
    else ()
        message(FATAL_ERROR "Unknown compiler frontend.")
    endif ()

    if (NOT PLATFORM_WEB)
        # content handling
        symlink_content(${EXECUTABLE} "contents")
    else ()
        # handle content when targeting the web, this is different 
        # from other platforms
    endif ()

    setup_target_compiler_definitions(${EXECUTABLE})

    # platform
    message(STATUS "Building in '${CMAKE_HOST_SYSTEM_NAME}' platform")
endfunction()

# ========== Dependencies (example only C++) ===========
# order matters for transitive dependencies
include("dependencies/fmt")
include("dependencies/tracy")

# ========== Main Executables ===========
set(CMAKE_CXX_STANDARD 20)
add_executable(cpp_project src/main.cpp)
target_include_directories(cpp_project PRIVATE "src")
target_link_libraries(cpp_project PRIVATE
        fmt::fmt
        Tracy::TracyClient
)
setup(cpp_project)

set(CMAKE_C_STANDARD 11)
add_executable(c_project src/main.c)
target_include_directories(cpp_project PRIVATE "src")
setup(c_project)
