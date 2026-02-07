enable_testing()

get_files(${CMAKE_CURRENT_SOURCE_DIR}/*.c c_files)
get_paths(${CMAKE_SOURCE_DIR}/*.h headers_paths)

add_executable(run_tests ${c_files})

target_include_directories(run_tests
    PRIVATE ${headers_paths}
)

target_link_libraries(run_tests
    ${PROJECT_NAME}
    unity
)

add_test(NAME ooc_tests COMMAND run_tests)
