#include "pedal/BLIT.hpp"

void BLIT::BLIT(){
  setFrequency(40.0f);
  setNumberOfHarmonics(std::floor(20000.0/frequency));
}

void BLIT::setFrequency(float newFrequency){
  frequency = newFrequency;
  phaseIncrement = (M_PI * 2.0f * periodInSamples)/
                   pdlSettings::sampleRate;
}
void BLIT::setNumberOfHarmonics(float newNumberOfHarmonics){
  if(newNumberOfHarmonics != newNumberOfHarmonics){
    periodInSamples = 2.0f * std::floor(numberOfHarmonics * 0.5) + 1;
    phaseIncrement = (M_PI * 2.0f * frequency * periodInSamples);
    float numberOfHarmonics = std::max(newNumberOfHarmonics, 1.0f);
  }
  