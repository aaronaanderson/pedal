//include functionality of a basic app
#include "example_app.hpp"

//include class if using

#include "pedal/ImpulseGenerator.hpp"
#include "pedal/WTSine.hpp"
#include "pedal/CTEnvelope.hpp"
#include "pedal/utilities.hpp"
#include "pedal/CircularBuffer.hpp"
#include "pedal/BufferTap.hpp"
#include "pedal/Delay.hpp"
#include <iostream>

#define NUM_CHIMES 20

float currentSample;
ImpulseGenerator trigger;
struct Chime{
  CTEnvelope envelope;
  WTSine oscillator;
};
Chime chimes[NUM_CHIMES];
CircularBuffer circularBuffer(10000.0f);
Delay delay;
//========================Audio Callback
void callback(float* out,float* in, unsigned buffer, unsigned rate, unsigned outputChannels,
              unsigned inputChannels, double time, pdlExampleApp* app) {
    
    trigger.setFrequency(pdlGetSlider(app, 0));
    trigger.setDeviation(1.0f - pdlGetSlider(app, 1));
    trigger.setMaskChance(pdlGetSlider(app, 4));
    delay.setDelayTime(pdlGetSlider(app, 5));
    delay.setFeedback(pdlGetSlider(app, 6));

    for(int i = 0; i < NUM_CHIMES; i++){
        chimes[i].envelope.setAttack(pdlGetSlider(app, 2));
        chimes[i].envelope.setRelease(pdlGetSlider(app,3));
    }
    //audio loop
    for (unsigned i = 0; i < buffer; i ++) {//for entire buffer of frames
      
      //if there is a new chime
      if(trigger.generateSample() == 1.0f){//if the impulse is 1.0
        //trigger a random chime
        for(int j = 0; j < 40; j++){//try 40 times to find a free random chime
            int index = rangedRandom(0.0f, float(NUM_CHIMES));
            if(chimes[index].envelope.isBusy() == false){
                float newFrequency = mtof((int)rangedRandom(40.0f, 110.0f));
                chimes[index].oscillator.setFrequency(newFrequency);
                chimes[index].oscillator.setAmplitude(rangedRandom(0.01, 0.7));
                chimes[index].envelope.setTrigger(true);
                break;
            } 
        }
      }

      float currentSample = 0.0f;
      //basically the 'processChime' function
      for(int j = 0; j < NUM_CHIMES;j++){
        float sample = chimes[j].oscillator.generateSample();
        sample *= chimes[j].envelope.generateSample();
        currentSample += sample;
      }
      
      currentSample += delay.insertSample(currentSample);
      currentSample *= 0.1f;
      for (unsigned j = 0; j < outputChannels; j += 1) {//for every sample in frame
        out[outputChannels * i + j] = currentSample * 0.1;
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
    for(int i = 0; i < NUM_CHIMES; i++){
      chimes[i].envelope.setMode(CTEnvelope::modes::AR);
    }
    pdlSettings::sampleRate = pdlExampleAppGetSamplingRate(app);
    pdlSettings::bufferSize = pdlExampleAppGetBufferSize(app);
    //delay.setMaximumFeedbackTime(4000.0f);//set maximum to 4 seconds
    
    // Add your GUI elements here
    pdlAddSlider(app, 0, "ChimesPerSecond", 0.0f, 200.0f, 35.0f);
    pdlAddSlider(app, 1, "Periodicity", 0.0f, 1.0f, 0.0f);

    pdlAddSlider(app, 2, "Attack", 2.0f, 200.0f, 13.0f);
    pdlAddSlider(app, 3, "Release", 5.0f, 1000.0f, 900.0f);
    pdlAddSlider(app, 4, "burstMask", 0.0f, 1.0f, 0.7f);
    pdlAddSlider(app, 5, "delayTime", 0.0f, 2000.0f, 300.0f);
    pdlAddSlider(app, 6, "feedback", 0.0f, 0.9999f, 0.95f);

    //begin the app--------
    pdlStartExampleApp(app);
    //pdlSettings::sampleRate = app->sampling_rate;
    while (pdlRunExampleApp(app)) {//run forever
        pdlUpdateExampleApp(app);//run the application
    }
    //the application has stopped running, 
    pdlDeleteExampleApp(app);//free the app from memory
}
