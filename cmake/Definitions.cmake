function(target_compiler_definitions TARGET)
    # helper closure to fixate the target
    function(custom_add_macro_definition CONDITION DEFINITION #[[OPT_MESSAGE:2]])
        set(OPT_MESSAGE ${ARGV2})
        conditional_add_macro_definition(${TARGET} ${CONDITION} ${DEFINITION} ${OPT_MESSAGE})
    endfunction()

    # build type
    custom_add_macro_definition(${CONFIG_IS_DEV} DEV_BUILD "Build type: Development")
    custom_add_macro_definition(${CONFIG_IS_SHIPPING} SHIPPING_BUILD "Build type: Shipping")
    custom_add_macro_definition(${CONFIG_IS_PROFILE} PROFILE_BUILD "Build type: Profile (Shipping with debug info)")

    # compiler type
    custom_add_macro_definition(${COMPILER_IS_GNU_LIKE} COMPILER_GNU_LIKE)
    custom_add_macro_definition(${COMPILER_IS_MSVC_LIKE} COMPILER_MSVC_LIKE)
    
    # compiler
    custom_add_macro_definition(${COMPILER_IS_CLANG} COMPILER_CLANG)
    custom_add_macro_definition(${COMPILER_IS_GCC} COMPILER_GCC)
    custom_add_macro_definition(${COMPILER_IS_MSVC} COMPILER_MSVC)
    
    # platform
    custom_add_macro_definition(${TARGET_PLATFORM_IS_WINDOWS} PLATFORM_WINDOWS)
    custom_add_macro_definition(${TARGET_PLATFORM_IS_LINUX} PLATFORM_LINUX)
    custom_add_macro_definition(${TARGET_PLATFORM_IS_MACOS} PLATFORM_MACOS)
    custom_add_macro_definition(${TARGET_PLATFORM_IS_WEB}  PLATFORM_WEB)
    
    # others
    custom_add_macro_definition(${CONFIG_IS_DEV} ENABLE_ASSERTIONS "Enabling assertions")
endfunction()