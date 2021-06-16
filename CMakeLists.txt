cmake_minimum_required(VERSION 3.10)

project(px)

find_package(PkgConfig REQUIRED)

pkg_check_modules(FFmpeg REQUIRED IMPORTED_TARGET
  libavutil
  libavformat)

add_library(px SHARED
  src/px/px.c
  src/px/video.c
  src/px/frame.c)

target_link_libraries(px PRIVATE PkgConfig::FFmpeg)
target_include_directories(px PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/src
  ${CMAKE_CURRENT_SOURCE_DIR}/include)


configure_file(px.pc.in ${PROJECT_BINARY_DIR}/px.pc @ONLY)

install(TARGETS px LIBRARY DESTINATION lib)
install(FILES
  include/px/px.h
  include/px/video.h
  include/px/frame.h
  DESTINATION include/px)
