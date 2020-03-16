//include functionality of a basic app
#include "example_app.hpp"

#include "pedal/pdlSettings.hpp"
#include "pedal/Compressor.hpp"
#include "pedal/Buffer.hpp"
#include "pedal/BufferPlayer.hpp"

Compressor compressor;
Buffer buffer;
BufferPlayer soundPlayer(&buffer);

//========================Audio Callback
void callback(float* out, unsigned buffer, unsigned rate, unsigned channel,
              double time, pdlExampleApp* app) {
    float mx, my; pdlGetCursorPos(app, &mx, &my);//obtain mouse x and y coordinates
    bool trigger = pdlGetToggle(app, 0);//trigger envelope with toggle
    float currentSample = 0;
    for (unsigned i = 0; i < buffer; i += 1) {//for entire buffer of frames
      //get the next sample from the soundPlayer
      currentSample = soundPlayer.getSample();
      //process the sample with the compressor
      //currentSample = compressor.process(currentSample);
      for (unsigned j = 0; j < channel; j += 1) {//for every sample in frame
        out[channel * i + j] = currentSample * 0.01f;
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
    
    buffer.loadSoundFile("ding.wav");//load a soundfile
    soundPlayer.setInterpolatoinMode(InterpolationMode::LINEAR);
    soundPlayer.setPlayMode(PlayMode::LOOP);// loop the file

    // Add your GUI elements here
    pdlAddSlider(app, 0, "Threshold", -60, 0.0f, -12.0f);
    pdlAddSlider(app, 1, "Ratio", 1.0f, 20.0f, 3.0f);
    pdlAddSlider(app, 2, "InputGain", -60.0f, 30.0f,0.8f);
    pdlAddSlider(app, 3, "Attack Time(ms)", -1.0f, 1.0f, 0.5f);
    pdlAddSlider(app, 4, "Release Time(ms)", -1.0f, 1.0f, 0.0f);

    //begin the app--------
    pdlStartExampleApp(app);
    //pdlSettings::sampleRate = app->sampling_rate;
    while (pdlRunExampleApp(app)) {//run forever
        pdlUpdateExampleApp(app);//run the application
    }
    //the application has stopped running, 
    pdlDeleteExampleApp(app);//free the app from memory
}