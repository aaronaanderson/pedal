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
#include "pedal/Compressor.hpp"
//DebugTool debugger;
Buffer testBuffer(4000.0f);//Initiate buffer with 10 seconds duration
//testBuffer.fillSineSweep();//breaks
BufferPlayer player(&testBuffer);
//MoorerReverb reverb;
//StreamedRMS rms;
BufferedRMS rms;
float panPosition;
Compressor compressor;
//========================Audio Callback
void callback(float* out,float* in, unsigned buffer, unsigned rate, unsigned outputChannels,
              unsigned inputChannels, double time, pdlExampleApp* app) {

  //testBuffer.fillSineSweep();//fails here, but delayed.
  bool writeFile = pdlGetTrigger(app, 0);
  if(writeFile){
 //   testBuffer.writeSoundFile("temp");
  }
  //std::cout << rms.getSample() << std::endl;
  player.setSpeed(pdlGetSlider(app, 0));

  panPosition = pdlGetSlider(app, 3);
  for (unsigned i = 0; i < buffer; i += 1) {//for entire buffer of frames
    //DebugTool::printOncePerBuffer(oscillator.getFrequency(), i);
    float leftSample = 0.0f;
    float rightSample = 0.0f;
    player.update();
    leftSample = player.getSample(0);
    float monoSum = leftSample;
    monoSum *= 0.701;
    monoSum = compressor.process(monoSum);
    float stereoFrame[2] = {0.0f, 0.0f};
    panStereo(monoSum, panPosition, stereoFrame);
    rms.process(monoSum);
    
    out[outputChannels * i] = stereoFrame[0];
    out[outputChannels * i + 1] = stereoFrame[1];
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
    
    player.setPlayMode(PlayMode::LOOP);
    player.setInterpolationMode(InterpolationMode::LINEAR);

    //begin the app--------
    pdlStartExampleApp(app);
    while (pdlRunExampleApp(app)) {//run forever
        pdlUpdateExampleApp(app);//run the application
    }
    //the application has stopped running, 
    pdlDeleteExampleApp(app);//free the app from memory
}