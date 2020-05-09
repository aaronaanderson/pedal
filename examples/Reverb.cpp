//include functionality of a basic app
#include "example_app.hpp"

//include class if using
#include "pedal/pdlSettings.hpp"
#include "pedal/FDNReverb.hpp"
#include "pedal/WhiteNoise.hpp"
#include "pedal/CREnvelope.hpp"
#include "pedal/ImpulseGenerator.hpp"

ImpulseGenerator impulse;
CREnvelope envelope;
WhiteNoise noise;
FDNReverb reverb;
//========================Audio Callback
void callback(float* out,float* in, unsigned bufferSize, unsigned rate, unsigned outputChannels,
              unsigned inputChannels, double time, pdlExampleApp* app) {
  
  for (unsigned i = 0; i < bufferSize; i += 1) {//for entire buffer of frames
    if(impulse.generateSample() == 1.0f){
        envelope.setTrigger(true);
    }
    float currentSample = noise.generateSample() * envelope.generateSample();
    currentSample = reverb.process(currentSample * 0.1f);
    for (unsigned j = 0; j < outputChannels; j += 1) {//for every sample in frame
      out[outputChannels * i + j] = currentSample;// * 0.1f;//deliver output to every channel
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
    
    impulse.setFrequency(1.0f);
    envelope.setMode(CREnvelope::modes::AR);
    envelope.setAttackTime(5.0f);
    envelope.setReleaseTime(10.0f);
    

    //begin the app--------
    pdlStartExampleApp(app);
    //pdlSettings::sampleRate = app->sampling_rate;
    while (pdlRunExampleApp(app)) {//run forever
        pdlUpdateExampleApp(app);//run the application
    }
    //the application has stopped running, 
    pdlDeleteExampleApp(app);//free the app from memory
}