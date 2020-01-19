#ifndef utilities_hpp
#define utilities_hpp

#include "stdlib.h"
#include "math.h"
#include "pdlSettings.hpp"
#include "algorithm"

float msToSamples(float time);
float samplesToMS(float samples);
float mtof(float midiValue);

//void normalize(float* inputBuffer, int bufferSize, float min, float max);//normalize data in place
float rangedRandom(float minimum, float maximum);

template<class T, class U, class V> 
T clamp(T input, U lowerBound, V upperBound){
  T result;
  if(input < lowerBound){
    result = lowerBound;
    return result;
  }else if(input > upperBound){
    result = upperBound;
    return result;
  }else{
    result = input;
    return result;
  }
}

 
#endif  