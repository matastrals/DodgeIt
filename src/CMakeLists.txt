add_library(DodgeIt STATIC
	${CMAKE_CURRENT_SOURCE_DIR}/Component.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/BulletEntity.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Game.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/PlayerEntity.cpp
 )

add_subdirectory(ecs)

target_include_directories(DodgeIt
	PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
)

add_executable(main_exe main.cpp)

target_link_libraries(
	DodgeIt PUBLIC ecs sfml::sfml
)

target_link_libraries(main_exe
	PUBLIC sfml::sfml DodgeIt
)

add_executable(main_test main_test.cpp)

target_link_libraries(main_test
	PRIVATE GTest::GTest GTest::Main 
	PUBLIC DodgeIt 
)

