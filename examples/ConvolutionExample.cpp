//include functionality of a basic app
#include "example_app.hpp"

#include "pedal/STFT.hpp"
#include "pedal/PinkNoise.hpp"
#include "pedal/TSine.hpp"
#include "pedal/utilities.hpp"//for SmoothValue
#include "pedal/MicroBenchmark.hpp"
PinkNoise pinkNoise;
TSine sineOsc;
SmoothValue<float> smoothFrequency;
STFT stftOne(512, 16);
STFT stftTwo(512, 16);
MicroBenchmark mB;
//========================Audio Callback
void callback(float* out, float* in, unsigned buffer, unsigned rate, unsigned outputChannels,
              unsigned inputChannels, double time, pdlExampleApp* app) {
    float mx, my; pdlGetCursorPos(app, &mx, &my);//obtain mouse x and y coordinates
    bool trigger = pdlGetToggle(app, 0);//trigger envelope with toggl
    //update frequency target with slider
    smoothFrequency.setTarget(std::pow(2.0f, pdlGetSlider(app, 0)));

    for (unsigned i = 0; i < buffer; i += 1) {//for entire buffer of frames
      sineOsc.setFrequency(smoothFrequency.process());//set frequency each sample
      float playerOneSample = sineOsc.generateSample() * 0.05f;
      float playerTwoSample = pinkNoise.generateSample() * 0.05f;//pinkNoise.generateSample() * 0.1f;
      mB.startTiming();
      stftOne.updateInput(playerOneSample);
      stftTwo.updateInput(playerTwoSample);
      
      if(stftOne.isFFTReady() && stftTwo.isFFTReady()){
        //multiply the magnitude of each bin
        for(int i = 0; i <= stftOne.getNumberOfBins(); i++){
          float newMagnitude = stftOne.getBinMagnitude(i) * stftTwo.getBinMagnitude(i);
          stftOne.setBinMagnitude(i, newMagnitude);
        }
      }
      stftOne.updateOutput();
      mB.stopTiming();
      out[outputChannels * i] = stftOne.getCurrentSample();
      out[outputChannels * i + 1] = stftOne.getCurrentSample();
    }
}
//======================main loop
int main() {
    //make an app (a pointer to an app, actually)
    pdlExampleApp* app = pdlInitExampleApp(callback);
    if (!app) {//if app doesn't succesfully allocate
      return 1;//cancel program, return 1
    }
    pdlSettings::sampleRate = pdlExampleAppGetSamplingRate(app);
    pdlSettings::bufferSize = pdlExampleAppGetBufferSize(app);
    mB.initialize("stft", 10000);
    // Add your GUI elements here
    pdlAddSlider(app, 0, "octave", 5.0f, 14.0f, 6.0f);
    
    //begin the app--------
    pdlStartExampleApp(app);
    //pdlSettings::sampleRate = app->sampling_rate;
    while (pdlRunExampleApp(app)) {//run forever
        pdlUpdateExampleApp(app);//run the application
    }
    if(mB.getCompleteFlag()){
      mB.printHighlites();
    }
    //the application has stopped running, 
    pdlDeleteExampleApp(app);//free the app from memory
}