ooc_collect_sources(ooc_sources ${CMAKE_CURRENT_SOURCE_DIR})
ooc_collect_include_dirs(ooc_include_dirs ${CMAKE_CURRENT_SOURCE_DIR})

add_library(${PROJECT_NAME} STATIC ${ooc_sources})

target_include_directories(${PROJECT_NAME}
    PUBLIC ${ooc_include_dirs}
)
