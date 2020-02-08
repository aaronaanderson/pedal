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
  }
  void setTarget(T newTarget){
    targetValue = newTarget;
  }
  void setTime(float newTime){
    arrivatlTime = newTime;
    calculateCoefficients();
  }
  T getCurrentValue(){

  }
  T getTargetValue(){

  }

  private:
  void calculateCoefficients(){
    a = exp(-M_2_PI / (arrivatlTime * 0.001f * pdlSettings::sampleRate));
    b = 1.0f - a;
  }
  T targetValue;
  T currentValue;
  float arrivatlTime;
  float a,b,z;//coefficients
};

#endif  

/*
class ParameterSmoother {
public:   
    ParameterSmoother(float smoothingTimeMS, int sampleRate) {
        smoothingTime = smoothingTimeMS;
        a = exp(-TWO_PI / (smoothingTimeMS * 0.001 * sampleRate));
        b = 1.0 - a;
        z = 0.0;
    }
    ParameterSmoother(float smoothingTimeMS){
        smoothingTime = smoothingTimeMS;
        a = exp(-TWO_PI / (smoothingTimeMS * 0.001 * ATKSettings::sampleRate));
        b = 1.0 - a;
        z = 0.0;
    }
    
    inline float process(float input) {
        z = (input * b) + (z * a);
        return z;
    }
    
    inline void setTime(float newSmoothingTime){
        //do a safety check since this is expensive; no need to recalculate if not changed
        if(smoothingTime != newSmoothingTime){
            smoothingTime = newSmoothingTime;
            a = exp(-TWO_PI / (smoothingTime * 0.001 * ATKSettings::sampleRate));
            b = 1.0 - a;
            //z = 0.0; this was wrong, if changing time keep previous z
        }
    }
    
private:
    float smoothingTime;
    float a;
    float b;
    float z;
};

struct SmoothValue {
    SmoothValue() = default;
    SmoothValue(float current, float target, float timeMS){
        currentValue = current;
        targetValue = target;
        smoother = new ParameterSmoother(timeMS);
    }
    float targetValue;
    float currentValue;
    ParameterSmoother* smoother;
    
    void process(){
        currentValue = smoother->process(targetValue);
    }
};
#endif /* Smoother_hpp */
