macro(maybe_find_system_package)
    message(STATUS "Searching for system-wide installation of ${CMAKE_FIND_PACKAGE_NAME}.")
    # Unset _FIND_REQUIRED if needed
    set(_BACKUP_${CMAKE_FIND_PACKAGE_NAME}_FIND_REQUIRED "${${CMAKE_FIND_PACKAGE_NAME}_FIND_REQUIRED}")
    unset(${CMAKE_FIND_PACKAGE_NAME}_FIND_REQUIRED)

    # Remove current path from module path to use system find_package
    set(_BACKUP_CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH})
    list(REMOVE_ITEM CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR})

    # Find package
    set(_MAYBE_COMPONENTS "")
    if(${CMAKE_FIND_PACKAGE_NAME}_FIND_COMPONENTS)
        set(_MAYBE_COMPONENTS "COMPONENTS")
    endif(${CMAKE_FIND_PACKAGE_NAME}_FIND_COMPONENTS)

    # use system find_package
    find_package(${CMAKE_FIND_PACKAGE_NAME} ${${CMAKE_FIND_PACKAGE_NAME}_FIND_VERSION} QUIET ${_MAYBE_COMPONENTS} ${${CMAKE_FIND_PACKAGE_NAME}_FIND_COMPONENTS})

    # Cleanup
    unset(${CMAKE_FIND_PACKAGE_NAME}_ERROR_REASON)

    set(CMAKE_MODULE_PATH ${_BACKUP_CMAKE_MODULE_PATH})
    unset(_BACKUP_CMAKE_MODULE_PATH)

    set(${CMAKE_FIND_PACKAGE_NAME}_FIND_REQUIRED ${_BACKUP_${CMAKE_FIND_PACKAGE_NAME}_FIND_REQUIRED})
    unset(_BACKUP_${CMAKE_FIND_PACKAGE_NAME}_FIND_REQUIRED)
    unset(_MAYBE_COMPONENTS)
endmacro(maybe_find_system_package)

macro(maybe_find_system_package_and_return)
    maybe_find_system_package(${CMAKE_FIND_PACKAGE_NAME})
    if(${CMAKE_FIND_PACKAGE_NAME}_FOUND)
        message(STATUS "${CMAKE_FIND_PACKAGE_NAME} was found on the system. Using it.")
        return()
    else(${CMAKE_FIND_PACKAGE_NAME}_FOUND)
        message(STATUS "${CMAKE_FIND_PACKAGE_NAME} was not found on the system.")
    endif(${CMAKE_FIND_PACKAGE_NAME}_FOUND)
endmacro(maybe_find_system_package_and_return)
