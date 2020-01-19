#ifndef utilities_hpp
#define utilities_hpp

#include "stdlib.h"
#include "math.h"
#include "pdlSettings.hpp"

float msToSamples(float time);
float samplesToMS(float samples);
float mtof(float midiValue);
float clamp(float input, float lowerBound, float upperBound);
void normalize(float* inputBuffer, int bufferSize, float min, float max);//normalize data in place

float rangedRandom(float minimum, float maximum);
 
#endif  