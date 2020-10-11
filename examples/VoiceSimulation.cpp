//include functionality of a basic app
#include "example_app.hpp"

//include class if using
#include "pedal/pdlSettings.hpp"
#include "pedal/VOSIM.hpp"

VOSIM voice;

//========================Audio Callback
void callback(float* output, float* input, int bufferSize, int inputChannels, int outputChannels, PedalExampleApp* app) {

  voice.setFrequency(pdlGetSlider(app, 0));
  voice.setFormantFrequency(pdlGetSlider(app, 1));
  voice.setDecayFactor(pdlGetSlider(app, 2));
  voice.setOscillationsPerPeriod(pdlGetSlider(app, 3));
  for (unsigned i = 0; i < bufferSize; i += 1) {//for entire buffer of frames
    float currentSample = voice.generateSample();
    for (unsigned j = 0; j < outputChannels; j += 1) {//for every sample in frame
      output[outputChannels * i + j] = currentSample * 0.1f;//deliver output to every channel
    }
  }
}
//======================main loop
int main() {
    //make an app (a pointer to an app, actually)
    PedalExampleApp* app = pdlInitializeExampleApp(callback);
    if (!app) {//if app doesn't succesfully allocate
      return 1;//cancel program, return 1
    }
    pdlSettings::sampleRate = pdlGetSampleRate(app);
    pdlSettings::bufferSize = pdlGetBufferSize(app);
    // Add your GUI elements here
    pdlAddSlider(app, 0, "Fundamental Frequency", 40.0f, 300.0f, 100.0f);
    pdlAddSlider(app, 1, "Formant Frequency", 40.0f, 5000.0f, 500.0f);
    pdlAddSlider(app, 2, "Decay Factor", 0.0f, 1.0f, 0.8);
    pdlAddSlider(app, 3, "Oscillations Per Period", 1, 40, 4);
    //begin the app--------
    pdlStartExampleApp(app);
    //pdlSettings::sampleRate = app->sampling_rate;
    while (pdlRunExampleApp(app)) {//run forever
        pdlUpdateExampleApp(app);//run the application
    }
    //the application has stopped running, 
    pdlDeleteExampleApp(app);//free the app from memory
}