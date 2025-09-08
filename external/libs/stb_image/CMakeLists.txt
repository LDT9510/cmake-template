project(stb_image
        VERSION 2.30
        DESCRIPTION "Image Loader"
        LANGUAGES C
)

add_library(stb_image STATIC src/stb_image_impl.cpp)

target_include_directories(stb_image PUBLIC include)

message(STATUS "stb_image setup correctly")
