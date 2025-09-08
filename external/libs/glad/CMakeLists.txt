project(glad LANGUAGES C)

message(STATUS "Generated with permalink https://gen.glad.sh/#generator=c&api=gl%3D3.3&profile=gl%3Dcore%2Cgles1%3Dcommon")
message(STATUS "OpenGL version 3.3 Core")

add_library(glad STATIC src/gl.c)
target_include_directories(glad PUBLIC include)

# workaround for <KHR/khrplatform.h> error using Clang on Windows
if (WIN32 AND (CMAKE_C_COMPILER_ID STREQUAL "Clang"))
    target_compile_definitions(glad PUBLIC -D__SCITECH_SNAP__=1)
endif ()