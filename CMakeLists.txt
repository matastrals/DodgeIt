cmake_minimum_required(VERSION 3.24)
project(DodgeIt LANGUAGES CXX C)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(build/conan_toolchain.cmake)

find_package(GTest REQUIRED)
find_package(SFML 2.6 REQUIRED COMPONENTS system window graphics)
enable_testing()

add_subdirectory(./src)

