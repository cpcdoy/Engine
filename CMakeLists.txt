cmake_minimum_required(VERSION 2.8)
project(Engine)

set(CMAKE_CXX_FLAGS "-Wall -Wextra -std=c++11 -pedantic -fPIC -pthread -ggdb3")

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS} -g")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS} -Werror -O3")

SET(CMAKE_AR  "gcc-ar")
SET(CMAKE_CXX_ARCHIVE_CREATE "<CMAKE_AR> qcs <TARGET> <LINK_FLAGS> <OBJECTS>")
SET(CMAKE_CXX_ARCHIVE_FINISH   true)

include_directories(src/debug)
include_directories(src/resource)
include_directories(src/ui)
include_directories(src/base)
include_directories(src/render_backend)
include_directories(src/engine)
include_directories(src/scene)
include_directories(src/utils)
#glsl optimizer
include_directories(3rd_party/glsl-optimizer/include)
include_directories(3rd_party/glsl-optimizer/src/mesa)
include_directories(3rd_party/glsl-optimizer/src/mapi)
include_directories(3rd_party/glsl-optimizer/src/glsl)
include_directories(3rd_party/glsl-optimizer/src)
#

add_subdirectory(src/debug)
add_subdirectory(src/resource)
add_subdirectory(src/ui)
add_subdirectory(src/base)
add_subdirectory(src/render_backend)
add_subdirectory(src/engine)
add_subdirectory(src/scene)
add_subdirectory(src/utils)

set(CMAKE_CTEST_COMMAND ctest -V)
add_custom_target(check COMMAND ${CMAKE_CTEST_COMMAND})

include_directories(tests/debug)
include_directories(3rd_party)

add_subdirectory(tests/debug)

file(GLOB_RECURSE glcpp-library_sources 3rd_party/glsl-optimizer/src/glsl/glcpp/*.c 3rd_party/glsl-optimizer/src/util/*.c)
add_library(glcpp-library ${glcpp-library_sources})

file(GLOB_RECURSE mesa_sources 3rd_party/glsl-optimizer/src/mesa/program/*.c 3rd_party/glsl-optimizer/src/mesa/main/*.c)
add_library(mesa ${mesa_sources})

file(GLOB_RECURSE glsl_sources 3rd_party/glsl-optimizer/src/glsl/*.cpp 3rd_party/glsl-optimizer/src/glsl/*.c)
file(GLOB_RECURSE glsl_sources_remove 3rd_party/glsl-optimizer/src/glsl/main.cpp 3rd_party/glsl-optimizer/src/glsl/builtin_stubs.cpp)
list(REMOVE_ITEM glsl_sources ${glsl_sources_remove})
add_library(glsl_optimizer ${glsl_sources})
target_link_libraries(glsl_optimizer glcpp-library mesa)
#


add_executable(Engine_check EXCLUDE_FROM_ALL
  tests/debug/main.cc
  $<TARGET_OBJECTS:debug> $<TARGET_OBJECTS:resource>
  $<TARGET_OBJECTS:ui> $<TARGET_OBJECTS:base>
  $<TARGET_OBJECTS:render_backend> $<TARGET_OBJECTS:engine_>
  $<TARGET_OBJECTS:scene> $<TARGET_OBJECTS:utils>)

add_library(Engine SHARED
  $<TARGET_OBJECTS:debug> $<TARGET_OBJECTS:resource>
  $<TARGET_OBJECTS:ui> $<TARGET_OBJECTS:base>
  $<TARGET_OBJECTS:render_backend> $<TARGET_OBJECTS:engine_>
  $<TARGET_OBJECTS:scene> $<TARGET_OBJECTS:utils>)

add_test(Engine_check Engine_check)
add_dependencies(check Engine_check)

target_link_libraries(Engine_check ${CMAKE_DL_LIBS} GL GLEW glfw SOIL glsl_optimizer)

add_custom_target(glsl-optimizer_parser_generation
  WORKING_DIRECTORY ./3rd_party/glsl-optimizer
  COMMAND ./generateParsers.sh
)
add_dependencies(glsl_optimizer glsl-optimizer_parser_generation)
#add_dependencies(Engine Engine_check glsl-optimizer_parser_generation)

add_custom_target(deep_clean
  COMMAND find . -iwholename '*cmake*' -not -name CMakeLists.txt -delete
  COMMAND find . -iwholename '*Makefile*' -delete
  COMMAND rm -rf Testing Engine_check libEngine.so doc/ libglcpp-library.a
  libmesa.a libglsl_optimizer.a
)

add_custom_target(doc
  WORKING_DIRECTORY src
  COMMAND doxygen ../Doxyfile
)
