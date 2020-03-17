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
    
    compressor.setThresholdDB(pdlGetSlider(app, 0));
    compressor.setRatio(pdlGetSlider(app, 1));
    compressor.setInputGainDB(pdlGetSlider(app, 2));
    compressor.setAttackTime(pdlGetSlider(app, 3));
    compressor.setReleaseTime(pdlGetSlider(app, 4));
    compressor.setLookAheadTime(pdlGetSlider(app, 5));
    compressor.setMakeUpGainDB(pdlGetSlider(app, 6));
    float currentSample = 0;
    for (unsigned i = 0; i < buffer; i += 1) {//for entire buffer of frames
      //get the next sample from the soundPlayer
      currentSample = soundPlayer.update();//update player and retrieve value
      currentSample = compressor.process(currentSample);
      //process the sample with the compressor
      //currentSample = compressor.process(currentSample);
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
    
    buffer.loadSoundFile("snare.wav");//load a soundfile
    soundPlayer.setInterpolationMode(InterpolationMode::LINEAR);
    soundPlayer.setPlayMode(PlayMode::LOOP);// loop the file

    // Add your GUI elements here
    pdlAddSlider(app, 0, "Threshold(dB)", -60, 0.0f, -12.0f);
    pdlAddSlider(app, 1, "Ratio", 1.0f, 100.0f, 3.0f);
    pdlAddSlider(app, 2, "InputGain(dB)", -12.0f, 30.0f, 6.0f);
    pdlAddSlider(app, 3, "Attack Time(ms)", 0.0f, 15.0f, 35.0f);
    pdlAddSlider(app, 4, "Release Time(ms)", 0.0f, 20.0f, 35.5f);
    pdlAddSlider(app, 5, "look-ahead(ms)", 0.0f, 50.0f, 20.0f);
    pdlAddSlider(app, 6, "Make-up Gain(dB)", 0.0f, 18.0f, 0.0f);

    //begin the app--------
    pdlStartExampleApp(app);
    //pdlSettings::sampleRate = app->sampling_rate;
    while (pdlRunExampleApp(app)) {//run forever
        pdlUpdateExampleApp(app);//run the application
    }
    //the application has stopped running, 
    pdlDeleteExampleApp(app);//free the app from memory
}