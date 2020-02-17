//include functionality of a basic app
#include "example_app.hpp"

//include class if using
#include "pedal/TSine.hpp"
#include "pedal/Buffer.hpp"
//#include <iostream>
//#include "pedal/DebugTool.hpp"
//#include "AudioFFT.h"
#include "pedal/BufferPlayer.hpp"

//DebugTool debugger;
Buffer testBuffer(4000.0f);//Initiate buffer with 10 seconds duration
//testBuffer.fillSineSweep();//breaks
BufferPlayer player(&testBuffer);
//========================Audio Callback
void callback(float* out, unsigned buffer, unsigned rate, unsigned channel,
              double time, pdlExampleApp* app) {

  //testBuffer.fillSineSweep();//fails here, but delayed.
  bool writeFile = pdlGetTrigger(app, 0);
  if(writeFile){
 //   testBuffer.writeSoundFile("temp");
  }
  player.setSpeed(pdlGetSlider(app, 0));
  for (unsigned i = 0; i < buffer; i += 1) {//for entire buffer of frames
    //DebugTool::printOncePerBuffer(oscillator.getFrequency(), i);
    float leftSample = 0.0f;
    float rightSample = 0.0f;
    player.update();
    leftSample = player.getSample(0);
    rightSample = player.getSample(1);
  
    out[channel * i] = leftSample;
    out[channel * i + 1] = rightSample;
  }
}
//======================main loop
int main() {
    pdlExampleApp* app = pdlInitExampleApp(callback);
    if (!app) {//if app doesn't succesfully allocate
      return 1;//cancel program, return 1
    }
    pdlSettings::sampleRate = pdlExampleAppGetSamplingRate(app);
    pdlSettings::bufferSize = pdlExampleAppGetBufferSize(app);

    const char* pathToSoundFile = "microbialSurfaces.wav";
    testBuffer.loadSoundFile(pathToSoundFile);//breaks here
    //make an app (a pointer to an app, actually)
    //testBuffer.writeSoundFile("temp");//danger!
    // Add your GUI elements here
    pdlAddSlider(app, 0, "Speed", -2.0f, 16.0f, 1.0f);
    //pdlAddToggle(app, 0, "loud", false);
    pdlAddTrigger(app, 0, "trigger");
    
    //begin the app--------
    pdlStartExampleApp(app);
    //pdlSettings::sampleRate = app->sampling_rate;
    while (pdlRunExampleApp(app)) {//run forever
        pdlUpdateExampleApp(app);//run the application
    }
    //the application has stopped running, 
    pdlDeleteExampleApp(app);//free the app from memory
}