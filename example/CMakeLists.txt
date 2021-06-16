cmake_minimum_required(VERSION 3.10)

project(example)

find_package(PkgConfig REQUIRED)
pkg_check_modules(PX REQUIRED IMPORTED_TARGET px)

add_executable(example main.c)
target_link_libraries(example PRIVATE PkgConfig::PX)
