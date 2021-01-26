#include "exampleAppSource/example_app.hpp"
#include <iostream>

#include "pedal/STFT.hpp"
#include "pedal/TrivialTriangle.hpp"
#include "pedal/WaveTableSaw.hpp"
#include "pedal/SmoothValue.hpp"

using namespace pedal;

WaveTableSaw triangle(100.0f);
STFT stftOne(2048, 4);
WaveTableSaw saw(25.0f);
STFT stftTwo(2048, 4);
SmoothValue<float> outputVolume;

void audioCallback(float* output, float* input, int bufferSize, int inputChannels, int outputChannels, app::PedalExampleApp* app){
  for(int sampleIndex = 0; sampleIndex < bufferSize; sampleIndex++){
    stftOne.updateInput(triangle.generateSample() * 0.7f );
    stftTwo.updateInput(saw.generateSample() * 0.7f);
    if(stftOne.isFFTReady() && stftTwo.isFFTReady()){
      for(int binIndex = 0; binIndex < stftOne.getNumberOfBins(); binIndex++){
        stftOne.setBin(binIndex, stftOne.getBin(binIndex) * stftTwo.getBin(binIndex));
      }
    }
    float currentSample = stftOne.updateOutput();
    for(int channelIndex = 0; channelIndex < outputChannels; channelIndex++){
      output[sampleIndex * outputChannels + channelIndex] = currentSample * 0.1f;
    }
  }
}   

int main(){
  //Create the application (an audio callback is required here)
  app::PedalExampleApp* app = app::initializeExampleApp(audioCallback, Settings::sampleRate, Settings::bufferSize);
  app::startApp(app);
  //This is the perpetual loop; it will keep going until the window is closed
  while(app::shouldContinue(app)){//while the window is still open
    app::update(app);//continue running the app
  }
  //If this point is reached, the application is closing
  app::freeMemory(app);
}