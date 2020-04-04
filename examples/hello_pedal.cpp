//include functionality of a basic app
#include "example_app.hpp"

//include class if using
#include "pedal/pdlSettings.hpp"
#include "pedal/TSine.hpp"
//#include "pedal/TSaw.hpp"
//#include "pedal/TSquare.hpp"
#include "pedal/TTriangle.hpp"
//#include "pedal/TSaw.hpp"
#include "pedal/CTEnvelope.hpp"
#include "pedal/WTSine.hpp"
#include "pedal/WTSaw.hpp"
#include "pedal/WTSquare.hpp"
#include "pedal/WTTriangle.hpp"
#include "pedal/ImpulseGenerator.hpp"
#include <iostream>
#include "pedal/Delay.hpp"
#include "pedal/DebugTool.hpp"
#include "pedal/WhiteNoise.hpp"
#include "pedal/PinkNoise.hpp"
#include "pedal/utilities.hpp"
#include "AudioFFT.h"
PinkNoise noise;
SmoothValue<float> frequency;
TSine oscillator;
//========================Audio Callback
void callback(float* out,float* in, unsigned buffer, unsigned rate, unsigned outputChannels,
              unsigned inputChannels, double time, pdlExampleApp* app) {
    float mx, my; pdlGetCursorPos(app, &mx, &my);//obtain mouse x and y coordinates
    oscillator.setFrequency(pdlGetSlider(app, 0));//set frequecy by slider

    bool trigger = pdlGetToggle(app, 0);//trigger envelope with toggle
    float frequencyTarget = pow(2,4.0f + mx * 10.0f);
    if(trigger){
     frequency.setTarget(frequencyTarget);
    }else{
      oscillator.setFrequency(frequencyTarget);//set frequecy by slider
    }
    

    for (unsigned i = 0; i < buffer; i += 1) {//for entire buffer of frames
      if(trigger){
       oscillator.setFrequency(frequency.process());
      }
      float currentSample = oscillator.generateSample();//assign the saw to current sample
      for (unsigned j = 0; j < outputChannels; j += 1) {//for every sample in frame
        out[outputChannels * i + j] = currentSample * 0.1f;
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
    pdlAddSlider(app, 0, "frequency", 0.1f, 40.0f, 3.0f);
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
