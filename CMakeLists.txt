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
    src/generators/ImpulseGenerator.cpp
    src/generators/envelopes/CTEnvelope.cpp
    src/Buffer.cpp
    src/CircularBuffer.cpp
    src/modifiers/delay/Delay.cpp
    src/modifiers/delay/BufferTap.cpp
    src/utilities/Interpolation.cpp
    src/utilities/utilities.cpp
    src/pdlSettings.cpp
    src/utilities/DebugTool.cpp
    src/generators/noise/WhiteNoise.cpp 
    src/generators/noise/PinkNoise.cpp
    src/modifiers/filters/Biquad.cpp
    src/BufferPlayer.cpp
    src/modifiers/filters/CombFilter.cpp
    src/modifiers/filters/AllPass.cpp
    src/modifiers/filters/LowPassCombFilter.cpp
    src/modifiers/filters/LowPass.cpp
    src/modifiers/filters/HighPass.cpp
    src/modifiers/reverb/MoorerReverb.cpp
    src/BufferedRMS.cpp
    src/StreamedRMS.cpp
    src/modifiers/dynamic/Compressor.cpp
    src/modifiers/dynamic/Gate.cpp
    src/spectral/STFT.cpp
    src/generators/envelopes/CREnvelope.cpp
    src/generators/oscillators/BLIT.cpp
    src/generators/Window.cpp
    src/utilities/MicroBenchmark.cpp
    external/AudioFFT.cpp
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
target_include_directories(pedal PUBLIC include external)
#target_include_directories(pedal PUBLIC external)
# Libraries to link when building this target
# target_link_libraries(pedal PUBLIC "")

# Also include examples and tests folder in this project
add_subdirectory(examples)
add_subdirectory(tests)
