#ifndef utilities_hpp
#define utilities_hpp

#include "stdlib.h"
#include "pdlConstants.hpp"
#include <cmath>
#include "Settings.hpp"
#include "algorithm"

namespace pedal{
float msToSamples(float timeInMS);
float midiNoteToPlaySpeed(int midiNoteValue, int normalizeToNote = 60);
float secondsToSamples(float timeInSeconds);
float samplesToMS(float samples);
float mtof(float midiValue);
template<class T>
T amplitudeToDB(T amplitude){
  return 20.0 * std::log10(amplitude);
}
template<class T>
T dBToAmplitude(T dB){
  return std::pow(10.0, dB/20.0);
}
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

template<class T>
T bpmToHz(T bpm){
  return bpm/60.0;
}
template<class T>
T hzToBpm(T hz){
  return hz * 60.0;
}

template<class T>
T map(T inputValue, T inputLowerBound, T inputUpperBound, T outputLowerBound, T outputUpperBound){
  return (inputValue - inputLowerBound)/(inputUpperBound - inputLowerBound) * (outputUpperBound - outputLowerBound) + outputLowerBound;
}

}//end pedal namespace
#endif