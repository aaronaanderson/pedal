#include "pedal/TSquare.hpp"

using namespace pedal;

//constructors and deconstructors
//=========================================================
TSquare::TSquare(){//default constructor
  setFrequency(440);//default frequency is 440
  setPhase(0.0);
  setAmplitude(1.0);
  setDutyCycle(0.5);
}

TSquare::TSquare(float frequency){//override constructor
  setFrequency(frequency);
  setPhase(0.0);
  setAmplitude(1.0);
  setDutyCycle(0.5);
}

TSquare::~TSquare(){}//when object is deleted

//primary mechanics of class
//=========================================================
float TSquare::generateSample(){//return a float even if you don't use it
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

//Getters and setters
//=========================================================
void TSquare::setFrequency(float newFrequency){
  frequency = newFrequency;
  phaseIncrement = (frequency)/pdlSettings::sampleRate;
}
void TSquare::setPhase(float newPhase){//expecting (0-2PI)
    phase = fmod(newPhase, pedal::TWOPI);//ensure 0-2PI
    phase -= pedal::PI;//now -PI to PI
    phase /= pedal::PI;//nown -1 to 1
    //we need an offset. 0.0 phase should be 0.0 result
}
void TSquare::setDutyCycle(float newDutyCycle){dutyCycle = newDutyCycle;}
void TSquare::setAmplitude(float newAmplitude){amplitude = newAmplitude;}

float TSquare::getFrequency(){return frequency;}
float TSquare::getAmplitude(){return amplitude;}
float TSquare::getSample(){return currentSample;}
