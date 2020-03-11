//include functionality of a basic app
#include "example_app.hpp"

//include class if using
#include "pedal/TSine.hpp"
#include "pedal/Buffer.hpp"
//#include <iostream>
//#include "pedal/DebugTool.hpp"
//#include "AudioFFT.h"
#include "pedal/BufferPlayer.hpp"
#include "pedal/MoorerReverb.hpp"
#include "pedal/BufferedRMS.hpp"
#include "pedal/StreamedRMS.hpp"
#include "pedal/utilities.hpp"

//DebugTool debugger;
Buffer testBuffer(4000.0f);//Initiate buffer with 10 seconds duration
//testBuffer.fillSineSweep();//breaks
BufferPlayer player(&testBuffer);
MoorerReverb reverb;
//StreamedRMS rms;
BufferedRMS rms;
float panPosition;
//========================Audio Callback
void callback(float* out, unsigned buffer, unsigned rate, unsigned channel,
              double time, pdlExampleApp* app) {

  //testBuffer.fillSineSweep();//fails here, but delayed.
  bool writeFile = pdlGetTrigger(app, 0);
  if(writeFile){
 //   testBuffer.writeSoundFile("temp");
  }
  std::cout << rms.getSample() << std::endl;
  player.setPlayMode((PlayMode)pdlGetDropDown(app, 0));
  player.setInterpolatoinMode((InterpolationMode)pdlGetDropDown(app, 1));
  player.setSpeed(pdlGetSlider(app, 0));

  reverb.setReverbTime(pdlGetSlider(app, 1));
  reverb.setDryWetMix(pdlGetSlider(app,2));
  panPosition = pdlGetSlider(app, 3);
  for (unsigned i = 0; i < buffer; i += 1) {//for entire buffer of frames
    //DebugTool::printOncePerBuffer(oscillator.getFrequency(), i);
    float leftSample = 0.0f;
    float rightSample = 0.0f;
    player.update();
    leftSample = player.getSample(0);
    rightSample = player.getSample(1);
    float monoSum = leftSample + rightSample;
    monoSum *= 0.1;
    monoSum = reverb.process(monoSum);
    float stereoFrame[2] = {0.0f, 0.0f};
    panStereo(monoSum, panPosition, stereoFrame);
    rms.process(monoSum);
    out[channel * i] = stereoFrame[0];
    out[channel * i + 1] = stereoFrame[1];
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

    const char* pathToSoundFile = "ding.wav";
    testBuffer.loadSoundFile(pathToSoundFile);//breaks here
    //make an app (a pointer to an app, actually)
    //testBuffer.writeSoundFile("temp");//danger!
    // Add your GUI elements here
    pdlAddSlider(app, 0, "Speed", -2.0f, 16.0f, 1.0f);
    pdlAddSlider(app, 1, "reverbTime", 20.0f, 60000.0f, 3000.0f);
    pdlAddSlider(app, 2, "dryWetMix", 0.0f, 1.0f, 1.0f);
    pdlAddSlider(app, 3, "pan", -1.0f, 1.0f, 0.0f);
    pdlAddToggle(app, 0, "play", false);
    pdlAddTrigger(app, 0, "trigger");
    
    char* modeMenuContent[]{"ONE_SHOT", "LOOP", "PING_PONG"};
    pdlAddDropDown(app, 0, "Mode", modeMenuContent, 3);

    char* interpolationMenuContent[]{"NONE", "LINEAR", "CUBIC"};
    pdlAddDropDown(app, 1, "Interpolation", interpolationMenuContent, 3);
    //begin the app--------
    pdlStartExampleApp(app);
    while (pdlRunExampleApp(app)) {//run forever
        pdlUpdateExampleApp(app);//run the application
    }
    //the application has stopped running, 
    pdlDeleteExampleApp(app);//free the app from memory
}