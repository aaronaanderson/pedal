# Specify minimum version for CMake project
cmake_minimum_required(VERSION 3.0)

# Name of the whole project: library, examples, and tests
project(pedal)

add_library(pedal STATIC
    src/pedal.cpp
    src/generators/oscillators/TSine.cpp
    src/generators/oscillators/TSaw.cpp
    src/generators/oscillators/TTriangle.cpp
    src/generators/oscillators/TSquare.cpp 
    src/generators/oscillators/TPhasor.cpp
    src/generators/oscillators/WTSine.cpp
    src/generators/oscillators/WTSaw.cpp
    src/generators/oscillators/WTTriangle.cpp
    src/generators/oscillators/WTSquare.cpp
    src/generators/envelopes/CTEnvelope.cpp
    src/generators/windows/HanningWindow.cpp
    src/Interpolation.cpp
    src/utilities.cpp
    src/pdlSettings.cpp
)

set_target_properties(pedal PROPERTIES
    DEBUG_POSTFIX d # When debug build, put `d` in the end of the output library file
    CXX_STANDARD 14 # The compiler should support at least C++14
    CXX_STANDARD_REQUIRED ON # Specified C++ version support is a must
    ARCHIVE_OUTPUT_DIRECTORY lib # Where to put output library file (in general)
    ARCHIVE_OUTPUT_DIRECTORY_DEBUG lib # For debug build
    ARCHIVE_OUTPUT_DIRECTORY_RELEASE lib # Release
)

# Compiler options to use
# target_compile_options(pedal PRIVATE "")

# Definitions for source file compilation,
# works same as `#define`ing the items in every file
# target_compile_definitions(pedal PUBLIC "")

# Directory to look for header files for `#include`
# `PUBLIC` means any target that links to this target will also have these items
target_include_directories(pedal PUBLIC include)

# Libraries to link when building this target
# target_link_libraries(pedal PUBLIC "")

# Also include examples and tests folder in this project
add_subdirectory(examples)
add_subdirectory(tests)
