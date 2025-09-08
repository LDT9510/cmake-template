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
function(add_macro_definition TARGET DEFINITION #[[OPT_MESSAGE]])
    set(OPT_MESSAGE ${ARGN})
    
    target_compile_definitions(${TARGET} PRIVATE ${DEFINITION})
    
    if (OPT_MESSAGE)
        message(STATUS ${OPT_MESSAGE})
    endif ()
endfunction()

#[[ Same as `add_macro_definitions` but only add the definition if ${CONDITION} is TRUE ]] 
function(conditional_add_macro_definition TARGET CONDITION DEFINITION #[[OPT_MESSAGE]])
    set(OPT_MESSAGE ${ARGN})

    if (CONDITION)
        add_macro_definition(${TARGET} ${DEFINITION} ${OPT_MESSAGE})
    endif ()
endfunction()

#[[ Assigns the result of a boolean expression to ${VAR} ]]
macro(assign_bool VAR)
    if(${ARGN})
        set(${VAR} ON)
    else()
        set(${VAR} OFF)
    endif()
endmacro()
