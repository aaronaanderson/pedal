#include "pedal/Phasor.hpp"

using namespace pedal;

//constructors and deconstructors
//=========================================================
Phasor::Phasor(){//default constructor
  setFrequency(440);//default frequency is 440
  setPhase(0.0);
}

Phasor::Phasor(float frequency){//override constructor
  setFrequency(frequency);
  setPhase(0.0);
}

Phasor::~Phasor(){}//when object is deleted

//primary mechanics of class
//=========================================================
float Phasor::generateSample(){//DANGER, returns range 0 to TWOPI; do not output directly
  phase += phaseIncrement;
  while(phase > pedal::PI){
    phase -= pedal::PI;
  }
  while(phase < 0.0){//to ensure that negative frequencies will work
    phase += pedal::PI;
  }
  return phase;
}

//=========================================================
void Phasor::setFrequency(float newFrequency){
  frequency = newFrequency;
  phaseIncrement = (frequency * pedal::TWOPI)/pdlSettings::sampleRate;
}
void Phasor::setPhase(float newPhase){//set phase (0 - 2PI)
  //convert the standard 0-2PI range to -1 to 1 to make calculations cheap
  phase = fmod(newPhase, pedal::TWOPI);//make sure phase is in the 0-twopi range
  phase -= pedal::PI;//now the range is (-pi, pi);
  phase /= pedal::PI;//now the phase is (-1, 1);
}
float Phasor::getFrequency(){return frequency;}
float Phasor::getSample(){return currentSample;}