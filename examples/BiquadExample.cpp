//include functionality of a basic app
#include "example_app.hpp"

#include "pedal/pdlSettings.hpp"
#include "pedal/WhiteNoise.hpp"
#include "pedal/Biquad.hpp"
#include "pedal/TSaw.hpp"

Biquad filter;
//WhiteNoise noise;
TSaw noise(50.0f);
//========================Audio Callback
void callback(float* out, unsigned buffer, unsigned rate, unsigned channel,
              double time, pdlExampleApp* app) {
    float mx, my; pdlGetCursorPos(app, &mx, &my);//obtain mouse x and y coordinates
    bool trigger = pdlGetToggle(app, 0);//trigger envelope with toggle

    filter.setBiquad((FilterType)pdlGetDropDown(app, 0),//mode
                     pdlGetSlider(app,0),//frequency
                     pdlGetSlider(app, 1),//q
                     pdlGetSlider(app, 2));//gain

    //filter.setFrequency(pdlGetSlider(app, 0));
    //filter.setQ(pdlGetSlider(app,1));
    //filter.setGain(pdlGetSlider(app, 2));
    for (unsigned i = 0; i < buffer; i += 1) {//for entire buffer of frames
      float currentSample = noise.generateSample();
      currentSample = filter.processSample(currentSample);
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
    pdlAddSlider(app, 0, "frequency", 40.0f, 6000.0f, 300.0f);
    pdlAddSlider(app, 1, "Q", 0.5f, 20.0f, 2.0f);
    pdlAddSlider(app, 2, "Gain", -60.0f, 30.0f,0.8f);

    char* modeMenuContent[]{"LowPass", "HighPass", "BandPass", "BandReject","Peak", "LowShelf", "HighShelf"};
    pdlAddDropDown(app, 0, "Mode", modeMenuContent, 7);
    
    //begin the app--------
    pdlStartExampleApp(app);
    //pdlSettings::sampleRate = app->sampling_rate;
    while (pdlRunExampleApp(app)) {//run forever
        pdlUpdateExampleApp(app);//run the application
    }
    //the application has stopped running, 
    pdlDeleteExampleApp(app);//free the app from memory
}