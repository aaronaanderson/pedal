//include functionality of a basic app
#include "example_app.hpp"

//include class if using
//#include "pedal/TSine.hpp"
//#include "pedal/TSaw.hpp"
//#include "pedal/TSquare.hpp"
#include "pedal/TTriangle.hpp"
//#include "pedal/TSaw.hpp"
#include "pedal/CTEnvelope.hpp"
#include "pedal/WTSine.hpp"

WTSine oscillator;
CTEnvelope envelope;
//========================Audio Callback
void callback(float* out, unsigned buffer, unsigned rate, unsigned channel,
              double time, pdlExampleApp* app) {
    oscillator.setFrequency(pdlGetSlider(app, 0));//set frequecy by slider
    envelope.setAttack(pdlGetSlider(app, 1));
    envelope.setDecay(pdlGetSlider(app, 2));
    envelope.setSustain(pdlGetSlider(app, 3));
    envelope.setRelease(pdlGetSlider(app, 4));

    bool trigger = pdlGetToggle(app, 0);//trigger envelope with toggle
    //bool trigger = pdlGetTrigger(app, 0);
    envelope.setTrigger(trigger);//set trigger to up or down, on or off, etc
    float mx, my; pdlGetCursorPos(app, &mx, &my);//obtain mouse x and y coordinates

    for (unsigned i = 0; i < buffer; i += 1) {//for entire buffer of frames
        float currentSample = oscillator.generateSample();//assign the saw to current sample
        currentSample *= envelope.generateSample();//scale current sample by envelope
        for (unsigned j = 0; j < channel; j += 1) {//for every sample in frame
          out[channel * i + j] = currentSample;
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
    pdlAddSlider(app, 0, "frequency", 0.1f, 4000.0f, 200.0f);

    pdlAddSlider(app, 1, "Attack", 2.0f, 300.0f, 80.0f);
    pdlAddSlider(app, 2, "Decay", 2.0f, 200.0f, 30.0f);
    pdlAddSlider(app, 3, "Sustain", 0.00f, 1.0f, 0.7f);
    pdlAddSlider(app, 4, "Release", 5.0f, 1000.0f, 200.0f);

    pdlAddToggle(app, 0, "loud", false);
    //pdlAddTrigger(app, 0, "trigger");
    
    //begin the app--------
    pdlStartExampleApp(app);
    //pdlSettings::sampleRate = app->sampling_rate;
    while (pdlRunExampleApp(app)) {//run forever
        pdlUpdateExampleApp(app);//run the application
    }
    //the application has stopped running, 
    pdlDeleteExampleApp(app);//free the app from memory
}
