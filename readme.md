# Pedal
  
As a computer music composer, I found the transition from domain-specific languages/environments such as SuperCollider, MaxMSP, or PD into C++ audio programming tremendously challenging. Resources for learning common practices are scattered throughout multiple academic domains, including mathematics and computer science. Without access to the luxuries of higher education, this task may have been impossible to overcome.

Pedal is my response to this predicament; a C++ audio library that favors clarity, modularity, and computational performance in that order.

Clarity: All classes are named clearly, follow a consistent design pattern, and forego base classes. Each document is treated as a lesson and thoroughly documented. Documentation includes not only what is being done and why, but also give context of the algorithm in recent history, how these classes are typically used, and where to look for more information. Pedal is verbose. The variable name has explicit and full meaning. Naming is consistent within class types. If two classes have identical methods (which occurs frequently as inheritance was omitted) they should be named identically.

Modularity: Pedal is designed in C++ to compile for PC, Macintosh, and Linux. The omission of base classes makes taking classes from the library trivial. Interdependence within Pedal is kept to a minimum to support this. Pedal also uses as few external libraries as possible, and when used the most portable option is chosen if performance is not dramatically reduced.

Performance: Clear code need not be slow, it usually just takes longer to write. We at all times choose the best method that will not break portability. In many cases, there are multiple methods that vary in accuracy and computational cost. These methods are contained in separate classes. The computational workload of a class is noted in the document when high. Hopefully real performance results and comparisons to other libraries will be available in the future.

Programming audio in C++ can open a tremendous amount of opportunities; I hope Pedal will bring these opportunities to more people.

## **BUILDING**


Pedal uses a CMake build system. If you do not have CMake, follow instructions here: https://cmake.org/install/ 

### **Linux and MacOS**

Use the terminal to clone pedal

```terminal
git clone https://github.com/aaronaanderson/pedal.git
```

By default cmake will generate Makefiles for Linux and MacOS. 

```terminal 
cd pedal
mkdir build 
cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
make
```
In these calls, we have changed directory to pedal, made a build folder, called cmake from that build folder, and then called make to execute the make files. 

### **Windows**


After installing cmake, Install Git Bash if you do not have it: https://gitforwindows.org/

Use Git Bash to clone pedal

```terminal
git clone https://github.com/aaronaanderson/pedal.git
```

Add a build folder for CMake's generated content

```terminal
cd pedal
mkdir build
cd build
```

If you do not have Visual Studio, get it here: https://visualstudio.microsoft.com/vs/

Assuming you are using Visual Studio 2019, run this cmake command in pedal's newly created build folder

```terminal 
cmake -G "Visual Studio 16 2019" -A x64 ..
```

This will generate Visual Studio project files. You can open these files, and build from there.


## **Examples**

Examples are built by default when pedal is compiled. These builds will be located in the pedal/examples/ExampleBuilds directory
