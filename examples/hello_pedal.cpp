//include functionality of a basic app
#include "example_app.hpp"

//include class if using
#include "pedal/TSine.hpp"
#include "pedal/TSaw.hpp"
#include "pedal/TSquare.hpp"
#include "pedal/TTriangle.hpp"
#include "pedal/TSaw.hpp"
#include "pedal/CTEnvelope.hpp"

TSine saw;
CTEnvelope envelope;
//========================Audio Callback
void callback(float* out, unsigned buffer, unsigned rate, unsigned channel,
              double time, pdlExampleApp* app) {
    saw.setFrequency(pdlGetSlider(app, 0));//set frequecy by slider
    bool trigger = pdlGetToggle(app, 0);//trigger envelope with toggle
    //bool trigger = pdlGetTrigger(app, 0);
    envelope.setTriger(trigger);//set trigger to up or down, on or off, etc
    float mx, my; pdlGetCursorPos(app, &mx, &my);//obtain mouse x and y coordinates

    for (unsigned i = 0; i < buffer; i += 1) {//for entire buffer of frames
        float currentSample = saw.generateSample();//assign the saw to current sample
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
    
    // Add your GUI elements here
    pdlAddSlider(app, 0, "freq", 0.0f, 1000.0f, 440.0f);
    pdlAddToggle(app, 0, "loud", false);
    pdlAddTrigger(app, 0, "trigger");
    
    //begin the app
    pdlStartExampleApp(app);
    while (pdlRunExampleApp(app)) {//run forever
        pdlUpdateExampleApp(app);//run the application
    }
    //the application has stopped running, 
    pdlDeleteExampleApp(app);//free the app from memory
}
