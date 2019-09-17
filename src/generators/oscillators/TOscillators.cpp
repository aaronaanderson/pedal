

#include "TOscillators.hpp"

pdlTSine::pdlTSine(){
    setFrequency(440);
    setPhase(0.0);
    setAmplitude(1.0);
}

void pdlTSine::setFrequency(float newFrequency){
    frequency = newFrequency;
    phaseIncrement = (frequency * 2.0 * M_PI)/pdlSettings::sampleRate;
}
void pdlTSine::setPhase(float newPhase){phase = newPhase;}
void pdlTSine::setAmplitude(float newAmplitude){amplitude = newAmplitude;}

float pdlTSine::getFrequency(){return frequency;}
float pdlTSine::getAmplitude(){return amplitude;}
float pdlTSine::getSample(){return currentSample;}
float* pdlTSine::getBlock(){return currentBlock;}
