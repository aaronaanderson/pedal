# Pedal
## Pedagogical Audio Library
  
As a computer music composer, I found the transition from domain-specific languages/environments such as SuperCollider, MaxMSP, or PD into C++ audio programming tremendously challenging. Resources for learning for learning common practices are scattered throughout multiple academic domains, including mathematics and computer science. Without access to the luxuries of higher education, this task may have been impossible to overcome. 

Pedal is my response to this predicament; a C++ audio library that favors clarity, modularity, and computational performance in that order.

Clarity: All classes are named clearly, follow a consistent design pattern, and forego base classes. Each document is treated as a lesson and thoroughly documented. Documentation includes not only what is being done and why, but also give context of the algorithm in recent history, how these classes are typically used, and where to look for more information. Pedal is verbose. The variable name has explicit and full meaning. Naming is consistent within class types. If two classes have identical methods (which occurs frequently as inheritance was omitted) they should be named identically.

Modularity: Pedal is designed in C++ to compile for PC, Macintosh, and Linux. The omission of base classes makes taking classes from the library trivial. Interdependence within Pedal is kept to a minimum to support this. Pedal also uses as few external libraries as possible, and when used the most portable option is chosen if performance is not dramatically reduced.

Performance: Clear code need not be slow, it just takes longer to write. We at all times choose the best method that will not break portability. In many cases, there are multiple methods that vary in accuracy and computational cost. These methods are contained in separate classes. The computational workload of a class is noted in the document when high. Hopefully real performance results and comparisons to other libraries will be available in the future.

Programming audio in C++ can open a tremendous amount of opportunities; I hope Pedal will bring these opportunities to more people.

## BUILDING

 cmake -G ${GENERATOR} -A ${PLATFORM} -DCMAKE_BUILD_TYPE=${BUILD_TYPE} -S ${PEDAL_DIR} -B ${BUILD_DIR}

For options, check https://cmake.org/cmake/help/latest/manual/cmake.1.html#options

For generators, check https://cmake.org/cmake/help/latest/manual/cmake-generators.7.html

Examples
   Assumes running command from Pedal root directory with build folder named 'build' inside the root directory

   Linux/MacOS + Make: cmake -DCMAKE_BUILD_TYPE=Debug -S . -B build
     Default generator is Make for both Linux and MacOS so can be skipped
   Windows + Visual Studio 2017 64 bit: cmake -G Visual Studio 15 2017 -A x64 -S . -B build
     Build type will be selected in Visual Studio so the option is not needed