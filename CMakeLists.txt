cmake_minimum_required(VERSION 3.28)

project(ooc
    VERSION 1.0.0.0
    LANGUAGES C
)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

enable_testing()

function(ooc_collect_sources out_var root_dir)
    file(GLOB_RECURSE collected_sources CONFIGURE_DEPENDS "${root_dir}/*.c")
    set(${out_var} ${collected_sources} PARENT_SCOPE)
endfunction()

function(ooc_collect_include_dirs out_var root_dir)
    file(GLOB_RECURSE collected_headers CONFIGURE_DEPENDS "${root_dir}/*.h")

    set(include_dirs "")
    foreach(header_file IN LISTS collected_headers)
        cmake_path(GET header_file PARENT_PATH header_dir)
        list(APPEND include_dirs "${header_dir}")
    endforeach()

    list(REMOVE_DUPLICATES include_dirs)
    set(${out_var} ${include_dirs} PARENT_SCOPE)
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
