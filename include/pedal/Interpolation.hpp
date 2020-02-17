#ifndef Interpolation_hpp
#define Interpolation_hpp

#include "math.h"
//the least expensive, yet still effective option for interpolation
inline float linearInterpolation(float inputSample, float previousSample, float nextSample){
  //determine position between previous and next sample
  float position = inputSample  - int(inputSample);
  //the result is a weighted sum based on this position
  return (previousSample * (1.0f-position)) + (nextSample * position);
}

float cubicInterpolation(float inputSample, float backTwo, float backOne, 
                         float forwardOne, float forwardTwo);

#endif