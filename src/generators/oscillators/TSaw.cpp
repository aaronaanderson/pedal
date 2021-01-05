#include "pedal/TSaw.hpp"

using namespace pedal;

//constructors and deconstructors
//=========================================================
TSaw::TSaw(){//default constructor
  setFrequency(440);//default frequency is 440
  setPhase(0.0);
  setAmplitude(1.0);
}

TSaw::TSaw(float frequency){//override constructor
  setFrequency(frequency);
  setPhase(0.0);
  setAmplitude(1.0);
}


TSaw::~TSaw(){}//deconstructor (called when object is deleted)

//primary mechanics of class
//=========================================================
float TSaw::generateSample(){//return a float even if you don't use it
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

//Getters and setters
//=========================================================
void TSaw::setFrequency(float newFrequency){
  frequency = newFrequency;
  phaseIncrement = (frequency * 2.0)/pdlSettings::sampleRate;
}
void TSaw::setPhase(float newPhase){//set phase (0 - 2PI)
  //convert the standard 0-2PI range to -1 to 1 to make calculations cheap
  phase = fmod(newPhase, pedal::TWOPI);//make sure phase is in the 0-twopi range
  phase -= pedal::PI;//now the range is (-pi, pi);
  phase /= pedal::PI;//now the phase is (-1, 1);
}
void TSaw::setAmplitude(float newAmplitude){amplitude = newAmplitude;}

float TSaw::getFrequency(){return frequency;}
float TSaw::getAmplitude(){return amplitude;}
float TSaw::getSample(){return currentSample;}
