//include functionality of a basic app
#include "example_app.hpp"

//include class if using
#include "pedal/pdlSettings.hpp"
#include "pedal/BLIT.hpp"
#include "pedal/HighPass.hpp"

BLIT blit;
HighPass highPass;
float currentSample = 0.0f;
//========================Audio Callback
void callback(float* out,float* in, unsigned bufferSize, unsigned rate, unsigned outputChannels,
              unsigned inputChannels, double time, pdlExampleApp* app) {
  
  float frequency = std::pow(2, pdlGetSlider(app, 0));
  blit.setFrequency(frequency);
  std::cout << "freq: " << frequency << "harmonics: " << blit.getNumberOfHarmonics() << std::endl;
 // blit.setNumberOfHarmonics(pdlGetSlider(app, 1));

  for (unsigned i = 0; i < bufferSize; i += 1) {//for entire buffer of frames
    float currentSample = highPass.process(blit.generateSample() - 0.5f) - (currentSample * pdlGetSlider(app, 2));
    
    for (unsigned j = 0; j < outputChannels; j += 1) {//for every sample in frame
      out[outputChannels * i + j] = currentSample * 0.1f;//deliver output to every channel
    }
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
    // Add your GUI elements here
    pdlAddSlider(app, 0, "Fundamental octave", 0.5f, 14.5f, 1.0f);
    pdlAddSlider(app, 1, "number Of Harmonics", 40.0f, 1000.0f, 500.0f);
    pdlAddSlider(app, 2, "g", 0.0f, 1.0f, 0.99f);
    highPass.setFrequency(1.0f);
    //begin the app--------
    pdlStartExampleApp(app);
    //pdlSettings::sampleRate = app->sampling_rate;
    while (pdlRunExampleApp(app)) {//run forever
      pdlUpdateExampleApp(app);//run the application
    }
    //the application has stopped running, 
    pdlDeleteExampleApp(app);//free the app from memory
}