function(setup_target_compiler_definitions TARGET)
    # helper closure to fixate the target
    function(custom_add_macro_definition CONDITION DEFINITION #[[OPT_MESSAGE]])
        set(OPT_MESSAGE ${ARGN})
        conditional_add_macro_definition(${TARGET} ${CONDITION} ${DEFINITION} ${OPT_MESSAGE})
    endfunction()

    # build type
    custom_add_macro_definition(${DEV_BUILD} DEV_BUILD "Build type: Development")
    custom_add_macro_definition(${SHIPPING_BUILD} SHIPPING_BUILD "Build type: Shipping")
    custom_add_macro_definition(${PROFILE_BUILD} PROFILE_BUILD "Build type: Profile (Shipping with debug info)")

    # compiler type
    custom_add_macro_definition(${COMPILER_GNU_LIKE} COMPILER_GNU_LIKE)
    custom_add_macro_definition(${COMPILER_MSVC_LIKE} COMPILER_MSVC_LIKE)

    # compiler
    custom_add_macro_definition(${COMPILER_CLANG} COMPILER_CLANG)
    custom_add_macro_definition(${COMPILER_GCC} COMPILER_GCC)
    custom_add_macro_definition(${COMPILER_MSVC} COMPILER_MSVC)

    # platform
    custom_add_macro_definition(${PLATFORM_WINDOWS} PLATFORM_WINDOWS)
    custom_add_macro_definition(${PLATFORM_LINUX} PLATFORM_LINUX)
    custom_add_macro_definition(${PLATFORM_MACOS} PLATFORM_MACOS)
    custom_add_macro_definition(${PLATFORM_WEB} PLATFORM_WEB)
    
    # others (just an example, not implemented)
    custom_add_macro_definition(${DEV_BUILD} ENABLE_ASSERTIONS "Enabling assertions")
endfunction()