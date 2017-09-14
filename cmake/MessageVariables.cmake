# Macros inspired by sakra's code: https://stackoverflow.com/a/9328525/3004221

# Prints all currently available variables and their values
macro(message_variables)
    get_cmake_property(_variableNames VARIABLES)
    foreach (_variableName ${_variableNames})
        message(STATUS "${_variableName}=${${_variableName}}")
    endforeach()
endmacro(message_variables)

# Prints all currently available variables containing ${PATTERN} in their name
# and their values.
macro(message_some_variables PATTERN)
    get_cmake_property(_variableNames VARIABLES)
    foreach (_variableName ${_variableNames})
        if(_variableName MATCHES ".*${PATTERN}.*")
            message(STATUS "${_variableName}=${${_variableName}}")
        endif(_variableName MATCHES ".*${PATTERN}.*")
    endforeach()
endmacro(message_some_variables)

# Prints all available environment variables.
macro(message_env_variables)
    execute_process(COMMAND "${CMAKE_COMMAND}" "-E" "environment")
endmacro(message_env_variables)
