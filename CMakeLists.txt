cmake_minimum_required(VERSION 2.6)
project(Engine CXX)

set(CMAKE_CXX_FLAGS "-Wall -Wextra -std=c++14 -pedantic -fPIC")

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS} -g")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS} -Werror -O3")

include_directories(src/debug)

add_subdirectory(src/debug)

set(CMAKE_CTEST_COMMAND ctest -V)
add_custom_target(check COMMAND ${CMAKE_CTEST_COMMAND})

include_directories(tests/debug)

add_subdirectory(tests/debug)

add_executable(Engine_check EXCLUDE_FROM_ALL
  tests/debug/main.cc
  $<TARGET_OBJECTS:debug>)

add_library(engine SHARED
  $<TARGET_OBJECTS:debug>)

add_test(Engine_check Engine_check)
add_dependencies(check Engine_check)

target_link_libraries(Engine_check ${CMAKE_DL_LIBS} GL GLEW glfw)

add_custom_target(deep_clean
  COMMAND find . -iwholename '*cmake*' -not -name CMakeLists.txt -delete
  COMMAND rm -rf Testing Engine_check libengine.so
)
