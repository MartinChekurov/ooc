ooc_collect_sources(unity_sources ${CMAKE_CURRENT_SOURCE_DIR})
ooc_collect_include_dirs(unity_include_dirs ${CMAKE_CURRENT_SOURCE_DIR})

add_library(unity STATIC ${unity_sources})

target_include_directories(unity
    PUBLIC ${unity_include_dirs}
)
