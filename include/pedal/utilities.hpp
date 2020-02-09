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

 /*
This class is a low pass filter for control
input. 
*/
template <class T>
class SmoothValue{
  public:
  SmoothValue(float initialTime = 50.0f){
    arrivalTime = initialTime;
    calculateCoefficients();
  }
  inline T process(){
    z = (targetValue * b) + (z * a);
    return z;
  }
  void setTime(float newTime){
    arrivalTime = newTime;
    calculateCoefficients();
  }
  void setTarget(T newTarget){targetValue = newTarget;}
  T getCurrentValue(){return currentValue;}
  T getTargetValue(){return targetValue;}

  private:
  void calculateCoefficients(){
    a = exp(-M_2_PI / (arrivalTime * 0.001f * pdlSettings::sampleRate));
    b = 1.0f - a;
  }
  T targetValue;
  T currentValue;
  float arrivalTime;
  float a,b;//coefficients
  T z;//storage for previous value
};
#endif