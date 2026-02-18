ooc_collect_sources(test_sources ${CMAKE_CURRENT_SOURCE_DIR})
ooc_collect_include_dirs(project_include_dirs ${CMAKE_SOURCE_DIR}/src)

add_executable(run_tests ${test_sources})

target_include_directories(run_tests
    PRIVATE ${project_include_dirs}
)

target_link_libraries(run_tests
    PRIVATE
        ${PROJECT_NAME}
        unity
)

add_test(NAME ooc_tests COMMAND run_tests)
