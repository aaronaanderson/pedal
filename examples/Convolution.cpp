#include "example_app.hpp"
#include <iostream>

#include "pedal/STFT.hpp"
#include "pedal/TrivialTriangle.hpp"
#include "pedal/WaveTableSaw.hpp"
#include "pedal/LowFrequencyOscillator.hpp"
#include "pedal/SmoothValue.hpp"

using namespace pedal;

TrivialTriangle triangle(100.0f);
STFT stftOne(16384, 128);
LowFrequencyOscillator lfoOne;
WaveTableSaw saw(40.0f);
STFT stftTwo(16384, 128);
LowFrequencyOscillator lfoTwo;
SmoothValue<float> outputVolume;

void audioCallback(float* output, float* input, int bufferSize, int inputChannels, int outputChannels, app::PedalExampleApp* app){
  for(int sampleIndex = 0; sampleIndex < bufferSize; sampleIndex++){
    triangle.setFrequency(lfoOne.generateSample());
    stftOne.updateInput(triangle.generateSample() );
    saw.setFrequency(lfoTwo.generateSample());
    stftTwo.updateInput(saw.generateSample());
    if(stftOne.isFFTReady() && stftTwo.isFFTReady()){
      for(int binIndex = 0; binIndex < stftOne.getNumberOfBins(); binIndex++){
        stftOne.setBin(binIndex, stftOne.getBin(binIndex) * stftTwo.getBin(binIndex));
      }
    }
    float currentSample = stftOne.updateOutput();
    for(int channelIndex = 0; channelIndex < outputChannels; channelIndex++){
      output[sampleIndex * outputChannels + channelIndex] = currentSample * 0.5f;
    }
  }
}   

int main(){
  //Create the application (an audio callback is required here)
  app::PedalExampleApp* app = app::pdlInitializeExampleApp(audioCallback, Settings::sampleRate, Settings::bufferSize);
  app::startApp(app);
  lfoOne.setWaveShape(LowFrequencyOscillator::WaveShape::Sine);
  lfoOne.setOutputRange(40.0f, 60.0f);
  lfoOne.setFrequency(0.33333f);
  lfoTwo.setWaveShape(LowFrequencyOscillator::WaveShape::Sine);
  lfoTwo.setOutputRange(80.0f, 200.0f);
  lfoTwo.setFrequency(0.1f);
  //This is the perpetual loop; it will keep going until the window is closed
  while(app::shouldContinue(app)){//while the window is still open
    app::update(app);//continue running the app
  }
  //If this point is reached, the application is closing
  app::freeMemory(app);
}