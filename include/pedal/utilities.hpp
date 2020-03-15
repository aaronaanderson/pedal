#ifndef utilities_hpp
#define utilities_hpp

#include "stdlib.h"
#define _USE_MATH_DEFINES
#include <cmath>
#include "pdlSettings.hpp"
#include "algorithm"

float msToSamples(float timeInMS);
float secondsToSamples(float timeInSeconds);
float samplesToMS(float samples);
float mtof(float midiValue);
float amplitudeToDB(float amplitude);
float dBToAmplitude(float dB);
//temporary stereo panner until spatialization system is added
void panStereo(float input, float position, float* outputFrame);
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
input. Typically control rate is lower than
audio rate. Direct use of control data results
in sudden change in the audio data which creates
undesired sounds.
*/
template <class T>  //The class is templated to allow a variety of data types
class SmoothValue{
  public:
  SmoothValue(float initialTime = 50.0f){//how much time, in ms, does it take to arrive (or approach target value)
    arrivalTime = initialTime;//store the input value (default to 50ms)
    calculateCoefficients();//calculate a and b (it is a lowpass filter)
    z = 0.0f;
  }
  inline T process(){//this function will be called per-sample on the data
    z = (targetValue * b) + (z * a);//when evaluated, z is the 'previous z' (it is leftover from last execution)
    return z;//return the new z value (the output sample)
  }
  void setTime(float newTime){//set time (in ms)
    arrivalTime = newTime;//store the input value
    calculateCoefficients();//calculate a and b
  }
  void setTarget(T newTarget){targetValue = newTarget;}
  T getCurrentValue(){return z;}
  T getTargetValue(){return targetValue;}
  float getTime(){return arrivalTime;}
  
  private:
  void calculateCoefficients(){//called only when 'setTime' is called (and in constructor)
    a = std::exp(-(M_PI * 2) / (arrivalTime * 0.001f * pdlSettings::sampleRate));//rearranged lpf coeff calculations
    b = 1.0f - a;
  }
  T targetValue;//what is the destination (of type T, determind by implementation)
  T currentValue;//how close to the destination? (output value)
  float arrivalTime;//how long to take
  float a,b;//coefficients
  T z;//storage for previous value
};

template <class t>
class DirectionalSmoothValue{
public:
  DirectionalSmoothValue(float initialTimeUp = 50.0f, float initialTimeDown = 50.0f){//how much time, in ms, does it take to arrive (or approach target value)
    arrivalTimeUp = initialTimeUp;//store the input value (default to 50ms)
    calculateUpCoefficients();//calculate aUp and bUp
    arrivalTimeDown = initialTimeDown;//store input value (default 50ms)
    claculateDownCoefficients();//calculate aDown and bDown
    z = 0.0f;//zero the 'previous' sample
    up = true;//set a default value, will be overwritten on first target value if needed
  }
  inline T process(){//this function will be called per-sample on the data
    if(up){
      //when evaluated, z is the 'previous z' (it is leftover from last execution)
      z = (targetValue * bUp) + (z * bUp);
    }else{//if going down/decending
      z = (targetValue * bDown) + (z * aDown);
    }
    
    return z;//return the new z value (the output sample)
  }
  void setTimeUp(float newTimeUp){//set time Up (in ms)
    arrivalTimeUp = newTimeUp;//store the input value
    calculateUpCoefficients();//calculate aUp and bUp
  }
  void setTimeDown(float newTimeDown){//set time down (in ms)
    arivalTimeDown = newTimeDown;//store the new value
    calculateDownCoefficients();//calculate aDown and bDown
  }
  void setTarget(T newTarget){//set a target value
    if(newTarget > z){//if target is greater than current value
      up = true;//then direction is up
    }else{//if target is <= current value
      down = true;//direction is down
    }
    targetValue = newTarget;//store new target
  }
  T getCurrentValue(){return z;}
  T getTargetValue(){return targetValue;} 
  float getTimeUp(){return arrivalTimeUp;}
  float getTimeDown(){return arrivalTimeDown;}

  private:
  void calculateUpCoefficients(){//called only when 'setTimeUp' is called (and in constructor)
    aUp = std::exp(-(M_PI * 2) / (arrivalTimeUp * 0.001f * pdlSettings::sampleRate));//rearranged lpf coeff calculations
    bUp = 1.0f - aUp;
  }
  void calculateDownCoefficients(){//called only when 'setTimeUp' is called (and in constructor)
    aDown = std::exp((M_PI * 2.0f) / (arrivalTimeDown * 0.001f * pdlSettings::sampleRate));
    bDown = 1.0f - aDown;
  }
  T targetValue;//what is the destination (of type T, determind by implementation)
  float arrivalTimeUp;//how long to take
  float arrivalTimeDown;
  bool up;//direction for storage: is value going up?
  float aUp,bUp;//coefficients if going up
  float aDown, bDown;//coefficients if going down
  T z;//storage for previous value
};
#endif