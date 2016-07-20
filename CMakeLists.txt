cmake_minimum_required(VERSION 2.6)
project(Engine CXX)

set(CMAKE_CXX_FLAGS "-Wall -Wextra -std=c++11 -pedantic -fPIC -O3")

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS} -g")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS} -Werror -O3")

include_directories(src/debug)
include_directories(src/resource)
include_directories(src/ui)
include_directories(src/base)
include_directories(src/render_backend)
include_directories(src/engine)
include_directories(src/scene)

add_subdirectory(src/debug)
add_subdirectory(src/resource)
add_subdirectory(src/ui)
add_subdirectory(src/base)
add_subdirectory(src/render_backend)
add_subdirectory(src/engine)
add_subdirectory(src/scene)

set(CMAKE_CTEST_COMMAND ctest -V)
add_custom_target(check COMMAND ${CMAKE_CTEST_COMMAND})

include_directories(tests/debug)

add_subdirectory(tests/debug)

add_executable(Engine_check EXCLUDE_FROM_ALL
  tests/debug/main.cc
  $<TARGET_OBJECTS:debug> $<TARGET_OBJECTS:resource>
  $<TARGET_OBJECTS:ui> $<TARGET_OBJECTS:base>
  $<TARGET_OBJECTS:render_backend> $<TARGET_OBJECTS:engine_>
  $<TARGET_OBJECTS:scene>)

add_library(Engine SHARED
  $<TARGET_OBJECTS:debug> $<TARGET_OBJECTS:resource>
  $<TARGET_OBJECTS:ui> $<TARGET_OBJECTS:base>
  $<TARGET_OBJECTS:render_backend> $<TARGET_OBJECTS:engine_>
  $<TARGET_OBJECTS:scene>)

add_test(Engine_check Engine_check)
add_dependencies(check Engine_check)

target_link_libraries(Engine_check ${CMAKE_DL_LIBS} GL GLEW glfw SOIL)

add_custom_target(deep_clean
  COMMAND find . -iwholename '*cmake*' -not -name CMakeLists.txt -delete
  COMMAND find . -iwholename '*Makefile*' -delete
  COMMAND rm -rf Testing Engine_check libEngine.so doc/
)

add_custom_target(doc
  WORKING_DIRECTORY src
  COMMAND doxygen ../Doxyfile
)
