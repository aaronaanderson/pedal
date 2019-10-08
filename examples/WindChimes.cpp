//include functionality of a basic app
#include "example_app.hpp"

//include class if using

#include "pedal/ImpulseGenerator.hpp"
#include "pedal/WTTriangle.hpp"
#include "pedal/CTEnvelope.hpp"
#include "pedal/utilities.hpp"

#define NUM_CHIMES 20

float currentSample;
ImpulseGenerator trigger;
struct Chime{
  CTEnvelope envelope;
  WTTriangle oscillator;
};
Chime chimes[NUM_CHIMES];
//========================Audio Callback
void callback(float* out, unsigned buffer, unsigned rate, unsigned channel,
              double time, pdlExampleApp* app) {
    
    trigger.setFrequency(pdlGetSlider(app, 0));
    trigger.setDeviation(1.0f - pdlGetSlider(app, 1));
    trigger.setMaskChance(pdlGetSlider(app, 4));
    for(int i = 0; i < NUM_CHIMES; i++){
        chimes[i].envelope.setAttack(pdlGetSlider(app, 2));
        chimes[i].envelope.setRelease(pdlGetSlider(app,3));
    }
    //audio loop
    for (unsigned i = 0; i < buffer; i ++) {//for entire buffer of frames
      
      if(trigger.generateSample() == 1.0f){//if the impulse is 1.0
        //trigger a random chime
        for(int j = 0; j < 40; j++){//try 40 times to find a free random chime
            int index = rangedRandom(0.0f, float(NUM_CHIMES));
            //std::cout << index << std::endl;
            if(chimes[index].envelope.isBusy() == false){
                chimes[index].envelope.setTrigger(true);
                break;
            }
        }
      }

      float currentSample = 0.0f;
      for(int j = 0; j < NUM_CHIMES;j++){
        currentSample += chimes[j].oscillator.generateSample();
        currentSample *= chimes[j].envelope.generateSample();
      }

      for (unsigned j = 0; j < channel; j += 1) {//for every sample in frame
        out[channel * i + j] = trigger.getSample() * 0.1;
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
      chimes[i].envelope.setMode(CTEnvelope::AR);
      float frequency = mtof(40.0f + (i*2.0f));
      std::cout << frequency << std::endl;
      chimes[i].oscillator.setFrequency(frequency);
      chimes[i].oscillator.setAmplitude(rangedRandom(0.001, 0.8));
    }

    pdlSettings::sampleRate = pdlExampleAppGetSamplingRate(app);
    pdlSettings::bufferSize = pdlExampleAppGetBufferSize(app);
    // Add your GUI elements here
    pdlAddSlider(app, 0, "ChimesPerSecond", 0.0f, 50.0f, 35.0f);
    pdlAddSlider(app, 1, "Periodicity", 0.0f, 1.0f, 0.0f);

    pdlAddSlider(app, 2, "Attack", 2.0f, 200.0f, 13.0f);
    pdlAddSlider(app, 3, "Release", 5.0f, 1000.0f, 900.0f);
    pdlAddSlider(app, 4, "burstMask", 0.0f, 1.0f, 0.7f);

    //begin the app--------
    pdlStartExampleApp(app);
    //pdlSettings::sampleRate = app->sampling_rate;
    while (pdlRunExampleApp(app)) {//run forever
        pdlUpdateExampleApp(app);//run the application
    }
    //the application has stopped running, 
    pdlDeleteExampleApp(app);//free the app from memory
}
