cmake_minimum_required(VERSION 3.24)
project(DodgeIt LANGUAGES CXX C)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(build/conan_toolchain.cmake)

find_package(GTest REQUIRED)
find_package(SFML 2.6 REQUIRED COMPONENTS system window graphics)
enable_testing()

add_subdirectory(./src)

# 1. Définir une règle d'installation pour copier les assets
install(DIRECTORY ${CMAKE_SOURCE_DIR}/assets
 DESTINATION .
 FILES_MATCHING
 PATTERN "*.*")
# 2. Ajouter une cible personnalisée pour exécuter l'installation
add_custom_target(copy_assets_to_runtime ALL
 COMMAND ${CMAKE_COMMAND} --install . --prefix
${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Release
 COMMENT "Installing assets to runtime directory"
)
