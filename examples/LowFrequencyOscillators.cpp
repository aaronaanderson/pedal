//include functionality of a basic app
#include "example_app.hpp"

//for samplerate and buffersize
#include "pedal/pdlSettings.hpp"
//trivial waveforms
#include "pedal/TPhasor.hpp"
#include "pedal/TSine.hpp"
#include "pedal/TSaw.hpp"
#include "pedal/TSquare.hpp" 
#include "pedal/TTriangle.hpp"
//Edge detector 
#include "pedal/EdgeDetector.hpp"
#include "pedal/CREnvelope.hpp"
#include "pedal/Counter.hpp"
#include "pedal/utilities.hpp"

TTriangle tri;
TPhasor phasor;
EdgeDetector edgeDetector;
CREnvelope envelope;

Counter counter(5);
int sequence[5] = {50, 52, 54, 57, 59};

//========================Audio Callback
void callback(float* output, float* input, int bufferSize, int inputChannels, int outputChannels, PedalExampleApp* app) {
  float x, y;
  pdlGetCursorPos(app, &x, &y);

  envelope.setAttackTime(x * 30.0f);
  envelope.setReleaseTime(y * 100.0f);
  phasor.setFrequency(pdlGetSlider(app, 0));
  for (unsigned i = 0; i < bufferSize; i += 1) {//for entire buffer of frames
    if(edgeDetector.process(phasor.generateSample())){
      envelope.setTrigger(true);
      tri.setFrequency(mtof(sequence[counter.increment()]));
    }
    float currentSample = tri.generateSample() * envelope.generateSample();
    for (unsigned j = 0; j < outputChannels; j += 1) {//for every sample in frame
      output[outputChannels * i + j] = currentSample;//deliver output to every channel
    }
  }
}
//======================main loop
int main() {
    //make an app (a pointer to an app, actually)
    PedalExampleApp* app = pdlInitializeExampleApp(callback);
    if (!app) {//if app doesn't succesfully allocate
      return 1;//cancel program, return 1
    }
    pdlSettings::sampleRate = pdlGetSampleRate(app);
    pdlSettings::bufferSize = pdlGetBufferSize(app);
    
    // Add your GUI elements here
    pdlAddSlider(app, 0, "Frequency", 0.0f, 100.0f, 2.0f);
    pdlAddSlider(app, 1, "delay time(ms)", 0.0f, 2000.0f, 500.0f);
    
    phasor.setFrequency(5.0f);

    envelope.setMode(CREnvelope::modes::AR);
    envelope.setAttackTime(20.0f);
    envelope.setReleaseTime(50.0f);
    //begin the app--------
    pdlStartExampleApp(app);
    //pdlSettings::sampleRate = app->sampling_rate;
    while (pdlRunExampleApp(app)) {//run forever
      pdlUpdateExampleApp(app);//run the application
    }
    //the application has stopped running, 
    pdlDeleteExampleApp(app);//free the app from memory
}