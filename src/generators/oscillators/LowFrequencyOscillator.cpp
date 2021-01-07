#include "pedal/LowFrequencyOscillator.hpp"

using namespace pedal;

#include <iostream>

LowFrequencyOscillator::LowFrequencyOscillator(){
    std::get<pedal::TSine*>(oscillator) = new TSine(880.0f);
   // oscillator.setFrequency(440.0f);
    currentWaveShape = WaveShape::Sine;
    (int)currentWaveShape;
    std::cout << oscillator.index() << std::endl;
}
LowFrequencyOscillator::~LowFrequencyOscillator(){
    delete std::get<pedal::TSine*>(oscillator);
}
float LowFrequencyOscillator::generateSample(){
    float currentSample = 0.0f;
    for()
    const int index = (const int)currentWaveShape;
    return (*std::get_if<index>(&oscillator))->generateSample();
}

void LowFrequencyOscillator::setFrequency(float newFrequency){
}
void LowFrequencyOscillator::setPhase(float newPhase){
}
void LowFrequencyOscillator::setAmplitude(float newAmplitude){
}
void LowFrequencyOscillator::flipWaveform(bool shouldFlipWaveform){
}
void LowFrequencyOscillator::setWaveShape(WaveShape newWaveShape){
}

float  LowFrequencyOscillator::getSample(){}
float  LowFrequencyOscillator::getFrequency(){}
double LowFrequencyOscillator::getPhase(){}
float  LowFrequencyOscillator::getAmplitude(){}
bool   LowFrequencyOscillator::getShouldFlipWaveform(){}
LowFrequencyOscillator::WaveShape LowFrequencyOscillator::getWaveShape(){}