#include "pedal/TPhasor.hpp"

using namespace pedal;

//constructors and deconstructors
//=========================================================
TPhasor::TPhasor(){//default constructor
  setFrequency(440);//default frequency is 440
  setPhase(0.0);
  setAmplitude(1.0);
}

TPhasor::TPhasor(float frequency){//override constructor
  setFrequency(frequency);
  setPhase(0.0);
  setAmplitude(1.0);
}

TPhasor::~TPhasor(){}//when object is deleted

//primary mechanics of class
//=========================================================
float TPhasor::generateSample(){//return a float even if you don't use it
  phase += phaseIncrement;
  while(phase > 1.0){
    phase -= 1.0;
  }
  while(phase < 0.0){//to ensure that negative frequencies will work
    phase += 1.0;
  }
  currentSample = phase * amplitude;
  return currentSample;
}

//Getters and setters
//=========================================================
void TPhasor::setFrequency(float newFrequency){
  frequency = newFrequency;
  phaseIncrement = frequency/pdlSettings::sampleRate;
}
void TPhasor::setPhase(float newPhase){//set phase (0 - 2PI)
  //convert the standard 0-2PI range to -1 to 1 to make calculations cheap
  phase = fmod(newPhase, pedal::TWOPI);//make sure phase is in the 0-twopi range
  phase -= pedal::PI;//now the range is (-pi, pi);
  phase /= pedal::PI;//now the phase is (-1, 1);
}
void TPhasor::setAmplitude(float newAmplitude){amplitude = newAmplitude;}

float TPhasor::getFrequency(){return frequency;}
float TPhasor::getAmplitude(){return amplitude;}
float TPhasor::getSample(){return currentSample;}
