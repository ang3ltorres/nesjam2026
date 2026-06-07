cmake_minimum_required(VERSION 3.21)

project(
  game
  VERSION 0.1.0
  LANGUAGES C
)

set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

file(
  GLOB_RECURSE SOURCES
  CONFIGURE_DEPENDS
  "${CMAKE_SOURCE_DIR}/src/*.c"
)

add_executable(game)

target_sources(game PRIVATE ${SOURCES})

target_include_directories(
  game
  PRIVATE
    "${CMAKE_SOURCE_DIR}/include"
)

target_compile_options(
  game
  PRIVATE
    -Wall
    -Wextra
    -Wpedantic
)

target_link_libraries(
  game
  PRIVATE
    raylib
    m
    pthread
    dl
    rt
    X11
)
