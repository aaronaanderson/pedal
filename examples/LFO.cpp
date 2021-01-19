#include "example_app.hpp"
#include <iostream>

#include "pedal/LowFrequencyOscillator.hpp"
#include "pedal/WTrivialSaw.hpp"
#include "pedal/MoogLadderFilter.hpp"

using namespace pedal;

WTrivialSaw saw;
MoogLadderFilter filter;
LowFrequencyOscillator lfo;
SmoothValue<float> frequency;

void keyboardCallback(int key, bool keyDown){
  switch(key){
    case '1':
      lfo.setWaveShape(LowFrequencyOscillator::WaveShape::Sine);
    break;
    case '2':
      lfo.setWaveShape(LowFrequencyOscillator::WaveShape::Triangle);
    break;
    case '3':
      lfo.setWaveShape(LowFrequencyOscillator::WaveShape::Saw);
    break;
    case '4':
      lfo.setWaveShape(LowFrequencyOscillator::WaveShape::Square);
    break;
  }
}

void audioCallback(float* output, float* input, int bufferSize, int inputChannels, int outputChannels, app::PedalExampleApp* app){
  frequency.setTarget(app::getSlider(app, 0));

  for(int sampleIndex = 0; sampleIndex < bufferSize; sampleIndex++){
    float currentSample = saw.generateSample();
    lfo.setFrequency(frequency.process());
    filter.setFrequency(lfo.generateSample());
    currentSample = filter.processSample(currentSample);
    for(int channelIndex = 0; channelIndex < outputChannels; channelIndex++){
      output[sampleIndex * outputChannels + channelIndex] = currentSample * 0.1f;
    }
  }
}

int main(){
  //Create the application (an audio callback is required here)
  app::PedalExampleApp* app = app::pdlInitializeExampleApp(audioCallback, Settings::sampleRate, Settings::bufferSize);
  app::setKeyboardCallback(keyboardCallback);
  saw.setFrequency(30.0f);
  lfo.setOutputRange(60.0f, 2000.0f);
  lfo.setFrequency(0.125f);
  filter.setResonance(0.94f);
  app::addSlider(app, 0, "LFO frequency", 0.0f, 18.0f, 2.0f);
  app::startApp(app);
  //This is the perpetual loop; it will keep going until the window is closed
  while(app::shouldContinue(app)){//while the window is still open
    app::update(app);//continue running the app
  }
  //If this point is reached, the application is closing
  app::freeMemory(app);
}