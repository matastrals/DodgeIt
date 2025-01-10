add_library(Game STATIC
	${CMAKE_CURRENT_SOURCE_DIR}/MovementComponent.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/BulletEntity.cpp
 )

target_include_directories(Game
	PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
)

add_executable(main_exe main.cpp)

target_link_libraries(main_exe
	PUBLIC sfml::sfml Game
)

add_executable(main_test main_test.cpp)

target_link_libraries(main_test
	PRIVATE GTest::GTest GTest::Main 
	PUBLIC Game 
)

