//include functionality of a basic app
#include "example_app.hpp"

//include class if using
#include "pedal/pdlSettings.hpp"
#include "pedal/Delay.hpp"
#include "pedal/utilities.hpp"
Delay delay;//feedback delay
SmoothValue<float> delayTime;//smooth changes in delay time

//========================Audio Callback
void callback(float* out,float* in, unsigned bufferSize, unsigned rate, unsigned outputChannels,
              unsigned inputChannels, double time, pdlExampleApp* app) {
  
  delay.setFeedback(pdlGetSlider(app, 0));//set feedback amplitude from 0th slider
  delayTime.setTarget(pdlGetSlider(app, 1));//tell smooth value to target 1st slider value

  for (unsigned i = 0; i < bufferSize; i += 1) {//for entire buffer of frames
    float liveInputSample = in[i * inputChannels];//get the input sample (on the 0th channel)
    delayTime.process();//advance SmoothValue
    //set delay time per-sample
    delay.setDelayTime(delayTime.getCurrentValue());
    delay.insertSample(liveInputSample);//advance the delay object with the input
    float currentSample = delay.getSample();//collect the output

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
    delayTime.setTime(500.0f);
    // Add your GUI elements here
    pdlAddSlider(app, 0, "feedback", 0.0f, 0.99f, 0.4f);
    pdlAddSlider(app, 1, "delay time(ms)", 0.0f, 2000.0f, 500.0f);

    //begin the app--------
    pdlStartExampleApp(app);
    //pdlSettings::sampleRate = app->sampling_rate;
    while (pdlRunExampleApp(app)) {//run forever
        pdlUpdateExampleApp(app);//run the application
    }
    //the application has stopped running, 
    pdlDeleteExampleApp(app);//free the app from memory
}