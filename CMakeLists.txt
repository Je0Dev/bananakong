cmake_minimum_required(VERSION 3.16)
project(bananakong C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

add_compile_options(-Wall -Wextra)

# --- raylib discovery: CONFIG -> pkg-config -> manual path lookup ---
find_package(raylib CONFIG QUIET)

if(NOT raylib_FOUND)
    find_package(PkgConfig QUIET)
    if(PKG_CONFIG_FOUND)
        pkg_check_modules(RAYLIB_PC QUIET raylib)
    endif()
endif()

if(raylib_FOUND)
    set(RAYLIB_TARGET raylib)
elseif(RAYLIB_PC_FOUND)
    add_library(raylib_imported INTERFACE IMPORTED)
    set_target_properties(raylib_imported PROPERTIES
        INTERFACE_INCLUDE_DIRECTORIES "${RAYLIB_PC_INCLUDE_DIRS}"
        INTERFACE_LINK_LIBRARIES "${RAYLIB_PC_LIBRARIES}")
    set(RAYLIB_TARGET raylib_imported)
else()
    find_path(RAYLIB_INCLUDE_DIR raylib.h)
    find_library(RAYLIB_LIBRARY raylib)
    if(NOT RAYLIB_INCLUDE_DIR OR NOT RAYLIB_LIBRARY)
        message(FATAL_ERROR "raylib not found. See README.md for install instructions.")
    endif()
    add_library(raylib_imported INTERFACE IMPORTED)
    set_target_properties(raylib_imported PROPERTIES
        INTERFACE_INCLUDE_DIRECTORIES "${RAYLIB_INCLUDE_DIR}"
        INTERFACE_LINK_LIBRARIES "${RAYLIB_LIBRARY}")
    set(RAYLIB_TARGET raylib_imported)
endif()

set(INCLUDE_DIRS src src/config src/core src/entities src/world)

# --- game executable ---
add_executable(bananakong
    src/main.c
    src/core/game.c
    src/core/game_draw.c
    src/core/physics.c
    src/entities/barrel.c
    src/entities/player.c
    src/entities/player_draw.c
    src/world/level.c
)
target_include_directories(bananakong PRIVATE ${INCLUDE_DIRS})
target_link_libraries(bananakong PRIVATE ${RAYLIB_TARGET} m)

# --- tests ---
enable_testing()
add_executable(test_bananakong
    tests/test_main.c
    tests/test_physics.c
    tests/test_level.c
    src/core/physics.c
    src/world/level.c
)
target_include_directories(test_bananakong PRIVATE ${INCLUDE_DIRS} tests)
target_link_libraries(test_bananakong PRIVATE ${RAYLIB_TARGET} m)
add_test(NAME bananakong_tests COMMAND test_bananakong)