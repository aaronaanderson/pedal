#ifndef utilities_hpp
#define utilities_hpp

#include "math.h"

float mtof(float midiValue);
float clamp(float input, float lowerBound, float upperBound);
void normalize(float* inputBuffer, int bufferSize, float min, float max);//normalize data in place

float rangedRandom(float minimum, float maximum);
#endif