#[[ Adds a subdirectory with a binary directory at with root at 
    "${CMAKE_BINARY_DIR}/external/${LIB_NAME}" and mark it as
    EXCLUDE_FROM_ALL and SYSTEM
]]
macro(add_external_subdirectory LIB_NAME)
    add_subdirectory(
            "${EXTERNAL_LIBS_SOURCES_DIR}/${LIB_NAME}" 
            "${CMAKE_BINARY_DIR}/external/${LIB_NAME}"
            EXCLUDE_FROM_ALL
            SYSTEM
    )
endmacro()
