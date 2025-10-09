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

    # assertions
    custom_add_macro_definition(${DEV_BUILD} ENABLE_ASSERTIONS "Enabling assertions")

    # logging
    custom_add_macro_definition(${FORCE_DISABLE_LOGGING} SPDLOG_ACTIVE_LEVEL=6 "Disabling all logging") # equivalent to SPDLOG_LEVEL_OFF
    custom_add_macro_definition(${DEV_BUILD} SPDLOG_ACTIVE_LEVEL=1 "Log level set to DEBUG") # equivalent to SPDLOG_LEVEL_DEBUG

    assign_bool(LOG_LEVEL_INFO SHIPPING_BUILD OR PROFILE_BUILD)
    custom_add_macro_definition(${LOG_LEVEL_INFO} SPDLOG_ACTIVE_LEVEL=2 "Log level set to INFO") # equivalent to SPDLOG_LEVEL_INFO
    
    custom_add_macro_definition(${SHIPPING_BUILD} SPDLOG_NO_SOURCE_LOC "Disabling source code showing in logs.")
endfunction()