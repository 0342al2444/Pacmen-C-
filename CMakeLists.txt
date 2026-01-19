cmake_minimum_required(VERSION 3.24)

project(Pacmen VERSION 0.1.0 LANGUAGES C CXX)

set(VCPKG_APPLOCAL_DEPS OFF)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(pacmen
    src/main.cpp
    src/core/Game.cpp
    src/entities/Ghost.cpp
    src/game/TileMap.cpp
    src/render/Renderer.cpp
    src/systems/GhostSystem.cpp
    src/systems/Input.cpp
)

target_include_directories(pacmen PRIVATE src)

find_package(raylib CONFIG REQUIRED)
target_link_libraries(pacmen PRIVATE raylib)

if (WIN32)
    target_compile_definitions(pacmen PRIVATE NOMINMAX)
endif()
