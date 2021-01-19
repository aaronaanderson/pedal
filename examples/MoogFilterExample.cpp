#include "example_app.hpp"

#include "pedal/MoogLadderFilter.hpp"
#include "pedal/WhiteNoise.hpp"
#include "pedal/utilities.hpp"

using namespace pedal;

MoogLadderFilter filter;
WhiteNoise noise;

void keyboardCallback(int key, bool keyDown){
  switch(key){
    case '1':
      filter.setMode(MoogLadderFilter::MODE::FOURPOLE_LOWPASS);
    break;
    case '2':
      filter.setMode(MoogLadderFilter::MODE::FOURPOLE_BANDPASS);
    break;
    case '3':
      filter.setMode(MoogLadderFilter::MODE::FOURPOLE_HIGHPASS);
    break;
    case '4':
      filter.setMode(MoogLadderFilter::MODE::TWOPOLE_LOWPASS);
    break;
    case '5':
      filter.setMode(MoogLadderFilter::MODE::TWOPOLE_BANDPASS);
    break;
    case '6':
      filter.setMode(MoogLadderFilter::MODE::TWOPOLE_HIGHPASS);
    break;
  }
}

void audioCallback(float* output, float* input, int bufferSize, int inputChannels, int outputChannels, PedalExampleApp* app){
  filter.setFrequency(pdlGetSlider(app, 0));
  filter.setResonance(1.0f - (1.0f /pdlGetSlider(app, 1)));
  float lazyAmplitude = dBToAmplitude(pdlGetSlider(app, 2));
  for(int sampleIndex = 0; sampleIndex < bufferSize; sampleIndex++){
    float currentSample = noise.generateSample();
    currentSample = filter.processSample(currentSample);
    for(int channelIndex = 0; channelIndex < outputChannels; channelIndex++){
      output[sampleIndex * outputChannels + channelIndex] = currentSample * lazyAmplitude;
    }
  }
}

int main(){
    //Create the application (an audio callback is required here)
    PedalExampleApp* app = pdlInitializeExampleApp(audioCallback, 48000, 512);
    //If using a qwerty keyboard callback, add it
    pdlSetKeyboardCallback(keyboardCallback);
  
    pdlAddSlider(app, 0, "Frequency", 20.0f, 400.0f, 100.0f);
    pdlAddSlider(app, 1, "Q", 1.0f, 20.0f, 4.0f);
    pdlAddSlider(app, 2, "Output Gain", -60.0f, 0.0f, -9.0f);
    pdlStartExampleApp(app);
    //This is the perpetual loop; it will keep going until the window is closed
    while(pdlRunExampleApp(app)){//while the window is still open
        pdlUpdateExampleApp(app);//continue running the app
    }
    //If this point is reached, the application is closing
    pdlDeleteExampleApp(app);
}