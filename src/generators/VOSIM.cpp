#include "pedal/VOSIM.hpp"

using namespace pedal;

VOSIM::VOSIM(){
    frequency = 80.0f;
    setFrequency(80.0f);
    updateFrequency();
    formantFrequency = 2000.0f;
    setFormantFrequency(2000.0f);
    updateFormantFrequency();
    oscillationsPerPeriod = nextOscillationsPerPeriod = 8;
    currentOscillation = 0;
    localAmplitude = 1.0f;
    oscillationPhase = 0.0f;
    periodPhase = 0.0f;
    decayFactor = 0.7f;
}

float VOSIM::generateSample(){
  if(currentOscillation < std::min(oscillationsPerPeriod, maxOscillationsPerPeriod)){
    currentSample = std::sin(oscillationPhase);//get a sin sample (0-PI)
    currentSample *= currentSample;//multiply by self for squared-sine
    currentSample *= localAmplitude;//scale by constant amount per sin half period
    oscillationPhase += oscillationPhaseIncrement;
    if(oscillationPhase >= pedal::PI){//if past PI
      localAmplitude *= decayFactor;//scale the decay factor again
      oscillationPhase -= pedal::PI;//rewind phasor
      currentOscillation++;//increment which oscillation is being calculated
    }
  }else{//no more sin curves, the rest of the waveform is silence
    currentSample = 0.0f;
  }
  //increment the phase of the full period (0 to 1)
  periodPhase += periodPhaseIncrement;
  if(periodPhase > 1.0f){//reset
    currentOscillation = 0;
    localAmplitude = 1.0f;
    periodPhase -= 1.0f;
    //update how many oscillations now if changed
    oscillationsPerPeriod = nextOscillationsPerPeriod; 
    bool changed = false;
    if(nextFrequency != frequency){
      updateFrequency();
      changed = true;
    }
    if(nextFormantFrequency != formantFrequency){
      updateFormantFrequency();
      changed = true;
    }
    if(changed){
      maxOscillationsPerPeriod = (Settings::sampleRate/frequency) / 
                                 (Settings::sampleRate/(formantFrequency * pedal::PI));
    }
  }
  return currentSample;
}
void VOSIM::setFrequency(float newFrequency){
  nextFrequency = std::max(newFrequency, 0.0f);
}
void VOSIM::updateFrequency(){
  //how many samples long is the full waveform?
  frequency = nextFrequency;
  periodPhaseIncrement = (frequency / Settings::sampleRate);
  maxOscillationsPerPeriod = (Settings::sampleRate/frequency) / 
                             (Settings::sampleRate/(formantFrequency * pedal::PI));
}
void VOSIM::setFormantFrequency(float newFormantFrequency){
  nextFormantFrequency = std::max(newFormantFrequency, 0.0f);
}
void VOSIM::updateFormantFrequency(){
  formantFrequency = nextFormantFrequency;
  oscillationPhaseIncrement = ((formantFrequency * pedal::PI) / 
                              Settings::sampleRate);

}
void VOSIM::setDecayFactor(float newDecayFactor){
  decayFactor = std::fmin(newDecayFactor, 1.0f);
}
void VOSIM::setOscillationsPerPeriod(int newOscillationsPerPeriod){
  nextOscillationsPerPeriod = std::max(newOscillationsPerPeriod, 1);
}