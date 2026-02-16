cmake_minimum_required(VERSION 3.28)
project(ooc
    VERSION 1.0.0.0
    LANGUAGES C
)

set(CMAKE_C_STANDARD 99)

function(get_files wildcard files)
    file(GLOB_RECURSE temp ${wildcard})
    set(${files} ${temp} PARENT_SCOPE)
endfunction()

function(get_paths wildcard paths)
    file(GLOB_RECURSE files ${wildcard})
    set(path_list "")
    foreach(file IN LISTS files)
        cmake_path(GET file PARENT_PATH path)
        list(APPEND path_list ${path})
    endforeach()
    set(${paths} ${path_list} PARENT_SCOPE)
endfunction()

add_subdirectory(src)
add_subdirectory(thirdParty/unity)
add_subdirectory(tests)

find_package(Doxygen)
if(DOXYGEN_FOUND)
    add_custom_target(doc
        COMMAND ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        COMMENT "Generating API documentation"
        VERBATIM
    )
endif()
