#include "pedal/TrivialSquare.hpp"

using namespace pedal;

//constructors and deconstructors
//=========================================================
TrivialSquare::TrivialSquare(){//default constructor
  setFrequency(440);//default frequency is 440
  setPhase(0.0);
  setAmplitude(1.0);
  setDutyCycle(0.5);
}

TrivialSquare::TrivialSquare(float frequency){//override constructor
  setFrequency(frequency);
  setPhase(0.0);
  setAmplitude(1.0);
  setDutyCycle(0.5);
}

TrivialSquare::~TrivialSquare(){}//when object is deleted

//primary mechanics of class
//=========================================================
float TrivialSquare::generateSample(){//return a float even if you don't use it
  phase += phaseIncrement;
  while(phase > 1.0){
    phase -= 1.0;
  }
  while(phase < 0.0){
    phase += 1.0;
  }

  if(phase > dutyCycle){
    currentSample = -1.0;
  }else{
    currentSample = 1.0;
  }
  return currentSample;
}
//expects an input phase int he range of 0 to TWOPI
float TrivialSquare::generateSample(float inputPhase){//return a float even if you don't use it
  //wrap phase to useful range then scale to 0.0 to 1.0
  setPhase(inputPhase);
  if(phase > dutyCycle){//phase stored locally 0.0 to 1.0
    currentSample = -1.0;
  }else{
    currentSample = 1.0;
  }
  return currentSample;
}
//Getters and setters
//=========================================================
void TrivialSquare::setFrequency(float newFrequency){
  frequency = newFrequency;
  phaseIncrement = (frequency)/Settings::sampleRate;
}
void TrivialSquare::setPhase(float newPhase){//expecting (0-2PI)
    phase = fmod(newPhase, pedal::TWOPI);//ensure 0-2PI
    phase *= pedal::PI_INVERSE;//convert range to 0.0 to 2.0
    phase *= 0.5;//phase stored locally 0.0 to 1.0
}
void TrivialSquare::setDutyCycle(float newDutyCycle){dutyCycle = newDutyCycle;}
void TrivialSquare::setAmplitude(float newAmplitude){amplitude = newAmplitude;}

float TrivialSquare::getFrequency(){return frequency;}
float TrivialSquare::getAmplitude(){return amplitude;}
float TrivialSquare::getSample(){return currentSample;}
