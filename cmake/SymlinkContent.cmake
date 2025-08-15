#[[ Creates a symlink to the content folder in the binary folder,
    allows to test the project without copying the contents next
    to the executable.
    
    Parameters
        TARGET_NAME: The name of the target, to obtain the binary folder.
        CONTENT_DIR_NAME: The path to the contents, relative the the project root.
]]
function(symlink_content TARGET_NAME CONTENT_DIR_NAME)
    set(CONTENT_DIR_PATH ${PROJECT_ROOT_DIR}/${CONTENT_DIR_NAME})
    get_filename_component(CONTENT_DIR ${CONTENT_DIR_PATH} NAME)

    add_custom_command(
            TARGET ${TARGET_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E create_symlink
            ${CONTENT_DIR_PATH}
            $<TARGET_FILE_DIR:${TARGET_NAME}>/${CONTENT_DIR}
            VERBATIM
            COMMENT "Creating symlink to '$<TARGET_FILE_DIR:${TARGET_NAME}>/${CONTENT_DIR}'"
            COMMAND_EXPAND_LISTS)
endfunction()
