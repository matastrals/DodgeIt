add_library(ecs STATIC
    ${CMAKE_CURRENT_SOURCE_DIR}/entity_manager.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/system_manager.cpp
)

target_include_directories(ecs PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})