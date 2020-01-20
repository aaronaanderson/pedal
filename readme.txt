
  PEDAL
  -----
# Pedagogical Audio Library
  
  (in progress) Pedal is a C++ audio library. It is intended to be both a versitle tool for programmers and a
  useful resource to learn C++ audio programming.

# BUILDING

 . cmake -G ${GENERATOR} -A ${PLATFORM} -DCMAKE_BUILD_TYPE=${BUILD_TYPE} -S ${PEDAL_DIR} -B ${BUILD_DIR}
 . For options, check https://cmake.org/cmake/help/latest/manual/cmake.1.html#options
 . For generators, check https://cmake.org/cmake/help/latest/manual/cmake-generators.7.html
 . Examples
   . Assumes running command from Pedal root directory with build folder named 'build' inside the root directory
   . Linux/MacOS + Make: cmake -DCMAKE_BUILD_TYPE=Debug -S . -B build
     . Default generator is Make for both Linux and MacOS so can be skipped
   . Windows + Visual Studio 2017 64 bit: cmake -G Visual Studio 15 2017 -A x64 -S . -B build
     . Build type will be selected in Visual Studio so the option is not needed
   . For Ninja generator see below section


# USING NINJA BUILD SYSTEM


# RUNNING EXAMPLES

 . Windows Visual Studio
 . Make
 . Ninja

