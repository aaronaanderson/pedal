//include functionality of a basic app
#include "example_app.hpp"

//include class if using
#include "pedal/pdlSettings.hpp"
#include "pedal/CTEnvelope.hpp"//constant time envelope
#include "pedal/CREnvelope.hpp"//constant rate envelope
#include "pedal/WTSaw.hpp"
#include "pedal/Biquad.hpp"//for resonant lowpass

CTEnvelope linearADSR;
CTEnvelope linearAR;
CREnvelope curvedADSR;
CREnvelope curvedAR;

Biquad resLowpass;//just because it's nice
WTSaw saw(180.0f);//wavetable sawtooth at 180Hz

//========================Audio Callback
void callback(float* out,float* in, unsigned bufferSize, unsigned rate, unsigned outputChannels,
              unsigned inputChannels, double time, PedalExampleApp* app) {
  //Set attack for all envelopes
  linearADSR.setAttackTime(pdlGetSlider(app, 0));
  linearAR.setAttackTime(pdlGetSlider(app, 0));
  curvedADSR.setAttackTime(pdlGetSlider(app, 0));
  curvedAR.setAttackTime(pdlGetSlider(app, 0));
  //set decay for applicable envelopes
  linearADSR.setDecayTime(pdlGetSlider(app, 1));
  curvedADSR.setDecayTime(pdlGetSlider(app, 1));
  //set sustain (linear) of applicable envelopes (using dBtoAmplitude)
  linearADSR.setSustainLevel(dBToAmplitude(pdlGetSlider(app, 2)));
  curvedADSR.setSustainLevel(dBToAmplitude(pdlGetSlider(app, 2)));
  //set release of all envelopes
  linearADSR.setReleaseTime(pdlGetSlider(app, 3));
  linearAR.setReleaseTime(pdlGetSlider(app, 3));
  curvedADSR.setReleaseTime(pdlGetSlider(app, 3));
  curvedAR.setReleaseTime(pdlGetSlider(app, 3));

  //check the toggles and triggers
  linearADSR.setTrigger(pdlGetToggle(app, 0));
  curvedADSR.setTrigger(pdlGetToggle(app, 1));
  if(pdlGetTrigger(app, 0)){
    linearAR.setTrigger(true);
  }
  if(pdlGetTrigger(app, 1)){
    curvedAR.setTrigger(true);
  }
  for (unsigned i = 0; i < bufferSize; i += 1) {//for entire buffer of frames
    saw.generateSample();//must be called every sample
    float currentSample = 0.0f; //we will accumulate all results to currentSample
    //multiply each envelope by the current sample of the sawtooth, and accumulate result to current sample
    currentSample += saw.getSample() * linearADSR.generateSample();
    currentSample += saw.getSample() * linearAR.generateSample();
    currentSample += saw.getSample() * curvedADSR.generateSample();
    currentSample += saw.getSample() * curvedAR.generateSample();

    for (unsigned j = 0; j < outputChannels; j += 1) {//for every sample in frame
      out[outputChannels * i + j] = currentSample;//deliver output to every channel
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
    
    linearADSR.setMode(CTEnvelope::modes::ADSR);
    linearAR.setMode(CTEnvelope::modes::AR);
    curvedADSR.setMode(CREnvelope::modes::ADSR);
    curvedAR.setMode(CREnvelope::modes::AR);
    
    pdlAddSlider(app, 0, "Attack", 0.0f, 1000.0f, 80.0f);
    pdlAddSlider(app, 1, "Decay", 0.0f, 500.0f, 35.0f);
    pdlAddSlider(app, 2, "Sustain(dB)", -18.0f, 0.0f, -6.0f);
    pdlAddSlider(app, 3, "Release", 0.0f, 4000.0f, 1200.0f);

    pdlAddTrigger(app, 0, "Linear AR Trigger");
    pdlAddToggle(app, 0, "linear ADSR ON/OFF", 0);
    pdlAddTrigger(app, 1, "Curved AR Trigger");
    pdlAddToggle(app, 1, "Curved ADSR ON/OFF", 0);
    
    //begin the app--------
    pdlStartExampleApp(app);
    //pdlSettings::sampleRate = app->sampling_rate;
    while (pdlRunExampleApp(app)) {//run forever
        pdlUpdateExampleApp(app);//run the application
    }
    //the application has stopped running, 
    pdlDeleteExampleApp(app);//free the app from memory
}