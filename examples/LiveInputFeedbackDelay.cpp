//include functionality of a basic app
#include "example_app.hpp"

//include class if using
#include "pedal/pdlSettings.hpp"

//========================Audio Callback
void callback(float* out,float* in, unsigned buffer, unsigned rate, unsigned channel,
              double time, pdlExampleApp* app) {
  for (unsigned i = 0; i < buffer; i += 1) {//for entire buffer of frames
    float currentSample = in[i];
    for (unsigned j = 0; j < channel; j += 1) {//for every sample in frame
      out[channel * i + j] = currentSample * 0.1f;
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
    pdlAddSlider(app, 0, "feedback", 0.0f, 0.99f, 0.7f);

    //begin the app--------
    pdlStartExampleApp(app);
    //pdlSettings::sampleRate = app->sampling_rate;
    while (pdlRunExampleApp(app)) {//run forever
        pdlUpdateExampleApp(app);//run the application
    }
    //the application has stopped running, 
    pdlDeleteExampleApp(app);//free the app from memory
}
