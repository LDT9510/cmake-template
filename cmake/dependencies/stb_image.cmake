include("Utils")
include("ExternalsUtils")

block()
    set(LIB_NAME "stb_image")
    announce("Configuring \"${LIB_NAME}\" external library.")

    add_external_subdirectory(${LIB_NAME})
endblock()
