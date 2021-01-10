
#include "pedal/TTriangle.hpp"

using namespace pedal;

//constructors and deconstructors
//=========================================================
TTriangle::TTriangle(){//default constructor
  setFrequency(440);//default frequency is 440
  setPhase(0.0);
  setAmplitude(1.0);
}

TTriangle::TTriangle(float frequency){//override constructor
  setFrequency(frequency);
  setPhase(0.0);
  setAmplitude(1.0);
}

TTriangle::~TTriangle(){}//when object is deleted

//primary mechanics of class
//=========================================================
float TTriangle::generateSample(){//return a float even if you don't use it
  //We can start by making a trivial sawtooth
  phase += phaseIncrement;
  while(phase > 1.0){
    phase -= 2.0;
  }
  while(phase < -1.0){//to ensure that negative frequencies will work
    phase += 2.0;
  }
  //then convert sawtooth to triangle by flipping the negative regions
  currentSample = (std::fabs(phase) * 2.0) - 1.0;//convert from saw to triangle
  currentSample *= amplitude;//scale by
  return currentSample;
}
//overload expecting phase in the range of 0 to TWOPI
float TTriangle::generateSample(float inputPhase){//return a float even if you don't use it
  //We can start by making a trivial sawtooth
  phase += phaseIncrement;
  while(phase > pedal::TWOPI){
    phase -= pedal::TWOPI;
  }
  while(phase < 0.0f){//to ensure that negative frequencies will work
    phase += pedal::TWOPI;
  }
  //then convert sawtooth to triangle by flipping the negative regions
  currentSample = (std::fabs(phase - pedal::PI) * pedal::PI_INVERSE * 2.0f) - 1.0;//convert from saw to triangle
  currentSample *= amplitude;//scale by
  return currentSample;
}
//Getters and setters
//=========================================================
void TTriangle::setFrequency(float newFrequency){
  frequency = newFrequency;
  phaseIncrement = (frequency * pedal::TWOPI )/pdlSettings::sampleRate;
}
void TTriangle::setPhase(float newPhase){//expecting (0-2PI)
  phase = fmod(newPhase, pedal::TWOPI);//ensure 0-2PI
}
void TTriangle::setAmplitude(float newAmplitude){amplitude = newAmplitude;}

float TTriangle::getFrequency(){return frequency;}
float TTriangle::getAmplitude(){return amplitude;}
float TTriangle::getSample(){return currentSample;}
