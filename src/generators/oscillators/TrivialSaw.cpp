#include "pedal/TrivialSaw.hpp"

using namespace pedal;

//constructors and deconstructors
//=========================================================
TrivialSaw::TrivialSaw(){//default constructor
  setFrequency(440);//default frequency is 440
  setPhase(0.0);
  setAmplitude(1.0);
}

TrivialSaw::TrivialSaw(float frequency){//override constructor
  setFrequency(frequency);
  setPhase(0.0);
  setAmplitude(1.0);
}


TrivialSaw::~TrivialSaw(){}//deconstructor (called when object is deleted)

//primary mechanics of class
//=========================================================
float TrivialSaw::generateSample(){//return a float even if you don't use it
  phase += (float)phaseIncrement;
  while(phase > 1.0){
    phase -= 2.0;
  }
  while(phase < -1.0){//to ensure that negative frequencies will work
    phase += 2.0;
  }
  currentSample = phase * amplitude;
  return currentSample;
}
//expects phase 0 to TWOPI
float TrivialSaw::generateSample(float inputPhase){//if phase provided, use instead
  phase = clamp(inputPhase, 0.0f, pedal::TWOPI);
  return (pedal::PI_INVERSE * phase) - 1.0f;
}

//Getters and setters
//=========================================================
void TrivialSaw::setFrequency(float newFrequency){
  frequency = newFrequency;
  phaseIncrement = (frequency * 2.0)/Settings::sampleRate;
}
void TrivialSaw::setPhase(float newPhase){//set phase (0 - 2PI)
  //convert the standard 0-2PI range to -1 to 1 to make calculations cheap
  phase = std::fmod(newPhase, pedal::TWOPI);//make sure phase is in the 0-twopi range
  phase *= pedal::PI_INVERSE;//divide by PI 
  phase -= 1.0f;//phase stored locally in range of 0.0f to 1.0f
}
void TrivialSaw::setAmplitude(float newAmplitude){amplitude = newAmplitude;}

float TrivialSaw::getFrequency(){return frequency;}
float TrivialSaw::getAmplitude(){return amplitude;}
float TrivialSaw::getSample(){return currentSample;}
