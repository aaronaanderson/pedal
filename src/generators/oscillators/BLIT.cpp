#include "pedal/BLIT.hpp"

BLIT::BLIT(){
  setFrequency(1.0f);
  setPhase(0.0f);
  setSyncHarmonicsToFrequency(true);//only generate non-aliasing harmonics
  phase = 0.0f;
}

float BLIT::generateSample(){
  //calculate denominator in advance to avoid divide / 0
  float denominator = std::sin(phase);
  if(denominator <= std::numeric_limits<float>::epsilon()){//avoid / 0
    currentSample = 1.0f;
  }else{
    currentSample = std::sin(phase * numberOfHarmonics) / 
                    (numberOfHarmonics * denominator);
  }
  phase += phaseIncrement;
  if(phase >= M_PI){
    phase -= M_PI;
  }
  return currentSample;
}
void BLIT::setNumberOfHarmonics(float newNumberOfHarmonics){
  numberOfHarmonics = std::fmax(newNumberOfHarmonics, 1.0f);
  numberOfHarmonics = std::floor(numberOfHarmonics * 0.5f) * 2.0f + 1.0f;
}
void BLIT::setFrequency(float newFrequency){
  frequency = newFrequency;
  phaseIncrement = (0.5f *M_PI * frequency) / pdlSettings::sampleRate;
  if(syncHarmonicsWithFrequency){
    setNumberOfHarmonics(20000.0f/frequency);
    std::cout << numberOfHarmonics << " : " << numberOfHarmonics * frequency << std::endl;
  }

}
void BLIT::setPhase(float newPhase){
  phase = std::fmod(newPhase, M_PI);
}
void BLIT::setSyncHarmonicsToFrequency(bool newSyncHarmonicsToFrequency){
  if(newSyncHarmonicsToFrequency){//if true, then calculate harmonics before nyquist
    setNumberOfHarmonics(20000.0f/frequency);
  }
  syncHarmonicsWithFrequency = newSyncHarmonicsToFrequency;
}