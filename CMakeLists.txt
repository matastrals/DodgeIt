cmake_minimum_required(VERSION 3.24)
project(DodgeIt LANGUAGES CXX C)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(build/conan_toolchain.cmake)

find_package(GTest REQUIRED)
enable_testing()

# Main executable
add_executable(main_exe main.cpp)
target_link_libraries(main_exe
	PRIVATE GTest::GTest GTest::Main
)