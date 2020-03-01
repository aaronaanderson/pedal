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
inline float linearInterpolation(float x, float x0, float y0, float x1, float y1){
  float scalar = (x - x0) / (x1 - x0);
  return y0 * (1.0f-scalar) + (y1 * scalar);
}

float cubicInterpolation(float inputSample, float backTwo, float backOne, 
                         float forwardOne, float forwardTwo);

#endif