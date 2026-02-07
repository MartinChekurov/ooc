get_files(${CMAKE_CURRENT_SOURCE_DIR}/*.c c_files)
get_paths(${CMAKE_CURRENT_SOURCE_DIR}/*.h headers_paths)

add_library(unity STATIC ${c_files})

target_include_directories(unity
    PRIVATE ${headers_paths}
)
