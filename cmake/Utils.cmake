#[[ Prints a message with a custom formatting. ]]
function(announce MSG)
    message("=========================================================")
    message("${MSG}")
    message("=========================================================")
endfunction()

#[[ Same as set(${NAME} ${VALUE} CACHE BOOL "" FORCE). ]]
macro(force_bool NAME VALUE)
    set(${NAME} ${VALUE} CACHE BOOL "" FORCE)
endmacro()

#[[ Same as set(${NAME} ${VALUE} CACHE STRING "" FORCE). ]]
macro(force_string NAME VALUE)
    set(${NAME} ${VALUE} CACHE STRING "" FORCE)
endmacro()

#[[ Prints every element from ${LIST_VAR}. ]]
function(print_list LIST_VAR)
    foreach (ITEM ${LIST_VAR})
        message(STATUS "DEBUG item: ${ITEM}")
    endforeach ()
endfunction()

# [[ Source code macro pre-definition, with an optional message printed. ]]
function(add_macro_definition TARGET DEFINITION #[[OPT_MESSAGE:2]])
    set(OPT_MESSAGE ${ARGV2})
    
    target_compile_definitions(${TARGET} PRIVATE ${DEFINITION})
    
    if (OPT_MESSAGE)
        message(STATUS ${OPT_MESSAGE})
    endif ()
endfunction()

#[[ Same as `add_macro_definitions` but only add the definition if ${CONDITION} is TRUE ]] 
function(conditional_add_macro_definition TARGET CONDITION DEFINITION #[[OPT_MESSAGE:3]])
    set(OPT_MESSAGE ${ARGV3})

    if (CONDITION)
        add_macro_definition(${TARGET} ${DEFINITION} ${OPT_MESSAGE})
    endif ()
endfunction()
