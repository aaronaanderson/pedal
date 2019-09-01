cmake_minimum_required(VERSION 3.0)
project(pedal)

add_library(pedal STATIC src/pedal.cpp)

set_target_properties(pedal PROPERTIES
  DEBUG_POSTFIX d
  CXX_STANDARD 14
  CXX_STANDARD_REQUIRED ON
)

target_compile_options(pedal PRIVATE "")

target_compile_definitions(pedal PUBLIC "")

target_include_directories(pedal PUBLIC include)

target_link_libraries(pedal PUBLIC "")

add_subdirectory(examples)