#include "example_app.hpp"
#include <iostream>

#include "pedal/STFT.hpp"
#include "pedal/TTriangle.hpp"
#include "pedal/WTSaw.hpp"
#include "pedal/LowFrequencyOscillator.hpp"
using namespace pedal;

TTriangle triangle(100.0f);
STFT stftOne(4096, 64);
LowFrequencyOscillator lfoOne;
WTSaw saw(40.0f);
STFT stftTwo(4096, 64);
LowFrequencyOscillator lfoTwo;
SmoothValue<float> outputVolume;

void audioCallback(float* output, float* input, int bufferSize, int inputChannels, int outputChannels, PedalExampleApp* app){
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
  PedalExampleApp* app = pdlInitializeExampleApp(audioCallback, pdlSettings::sampleRate, pdlSettings::bufferSize);
  pdlStartExampleApp(app);
  lfoOne.setWaveShape(LowFrequencyOscillator::WaveShape::Sine);
  lfoOne.setOutputRange(40.0f, 60.0f);
  lfoOne.setFrequency(0.33333f);
  lfoTwo.setWaveShape(LowFrequencyOscillator::WaveShape::Sine);
  lfoTwo.setOutputRange(80.0f, 200.0f);
  lfoTwo.setFrequency(0.1f);
  //This is the perpetual loop; it will keep going until the window is closed
  while(pdlRunExampleApp(app)){//while the window is still open
    pdlUpdateExampleApp(app);//continue running the app
  }
  //If this point is reached, the application is closing
  pdlDeleteExampleApp(app);
}