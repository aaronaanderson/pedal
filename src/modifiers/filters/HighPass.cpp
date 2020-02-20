#include "pedal/HighPass.hpp"

HighPass::HighPass(float frequency){
  setFrequency(frequency);
  currentSample = 0.0f;
}

//float LowPass::process(float input) is in the header
//because it is inlined

void HighPass::setFrequency(float newFrequency){
  //These coefficient calculations are from Will Pirkle's
  //Designing Audio Effect Plug-ins in C++, pg 165
  float theta = (M_2_PI * newFrequency)/pdlSettings::sampleRate;
  float gamma = 2.0f + cos(theta);
  b = gamma - sqrt((gamma * gamma) - 1.0f);
  a = 1.0f - b;
  std::cout << M_2_PI << "/" << pdlSettings::sampleRate << " " << a << " " << b << std::endl;
}
float HighPass::getSample(){return currentSample;}