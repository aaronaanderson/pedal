//include functionality of a basic app
#include "example_app.hpp"

//include class if using
#include "pedal/WTSine.hpp"
#include "pedal/WTSaw.hpp"
#include "pedal/WTSquare.hpp"
#include "pedal/WTTriangle.hpp"
#include "pedal/WTSaw.hpp"
#include "pedal/CTEnvelope.hpp"
#include "pedal/MoorerReverb.hpp"
#include "pedal/CREnvelope.hpp"
#include "pedal/BLIT.hpp"

float currentSample;
WTTriangle triangle;
WTSine sine;
WTSquare square;
WTSaw saw;
MoorerReverb reverb;
CTEnvelope sustainedEnvelope;
CTEnvelope percussiveEnvelope;
CREnvelope fancyEnvelope;

BLIT blit;
//========================Audio Callback
void callback(float* out,float* in, unsigned buffer, unsigned rate, unsigned outputChannels,
              unsigned inputChannels, double time, pdlExampleApp* app) {
    
    sine.setFrequency(pdlGetSlider(app, 9));//set frequecy by slider
    triangle.setFrequency(pdlGetSlider(app, 9));//set frequecy by slider
    square.setFrequency(pdlGetSlider(app, 9));//set frequecy by slider
    saw.setFrequency(pdlGetSlider(app, 9));//set frequecy by slider
    blit.setFrequency(pdlGetSlider(app, 9));
    reverb.setDryWetMix(pdlGetSlider(app, 10));
    //envelopes are ADSR by default
    sustainedEnvelope.setAttack(pdlGetSlider(app, 5));
    sustainedEnvelope.setDecay(pdlGetSlider(app, 6));
    sustainedEnvelope.setSustain(pdlGetSlider(app, 7));
    sustainedEnvelope.setRelease(pdlGetSlider(app, 8));
    
    percussiveEnvelope.setMode(CTEnvelope::modes::AR);//set to attack-release
    percussiveEnvelope.setAttack(pdlGetSlider(app, 5));
    percussiveEnvelope.setRelease(pdlGetSlider(app, 8));
    
    fancyEnvelope.setAttackTime(pdlGetSlider(app, 5));
    fancyEnvelope.setDecayTime(pdlGetSlider(app, 6));
    fancyEnvelope.setSustainLevel(pdlGetSlider(app, 7));
    fancyEnvelope.setReleaseTime(pdlGetSlider(app, 8));
    
    bool trigger = pdlGetToggle(app, 0);//trigger envelope with toggle
    //bool trigger = pdlGetTrigger(app, 0);
    sustainedEnvelope.setTrigger(trigger);//set trigger to up or down, on or off, etc
    
    trigger = pdlGetTrigger(app, 0);
    percussiveEnvelope.setTrigger(trigger);

    trigger = pdlGetToggle(app, 1);
    fancyEnvelope.setTrigger(trigger);
    float mx, my; pdlGetCursorPos(app, &mx, &my);//obtain mouse x and y coordinates

    //audio loop
    for (unsigned i = 0; i < buffer; i += 1) {//for entire buffer of frames
      //generate the envelope samples
      float envelopeOne = sustainedEnvelope.generateSample();
      float envelopeTwo = percussiveEnvelope.generateSample();
       
      float resultOne = 0.0f;//create a float for envelope one
      //add together the result of all oscillators with slider values
      resultOne += sine.generateSample() * pdlGetSlider(app, 0);
      resultOne += saw.generateSample() * pdlGetSlider(app, 1); 
      resultOne += triangle.generateSample() * pdlGetSlider(app, 2);
      resultOne += square.generateSample() * pdlGetSlider(app, 3);
      resultOne *= envelopeOne;//scale by the current value of the envelope
      
      float resultTwo = 0.0f;
      resultTwo += sine.getSample() * pdlGetSlider(app, 0);
      resultTwo += saw.getSample() * pdlGetSlider(app, 1); 
      resultTwo += triangle.getSample() * pdlGetSlider(app, 2);
      resultTwo += square.getSample() * pdlGetSlider(app, 3);
      resultTwo *= envelopeTwo;
      
      float resultThree = 0.0f;
      resultThree += sine.getSample() * pdlGetSlider(app, 0);
      resultThree += saw.getSample() * pdlGetSlider(app, 1); 
      resultThree += triangle.getSample() * pdlGetSlider(app, 2);
      resultThree += square.getSample() * pdlGetSlider(app, 3);
      resultThree *= fancyEnvelope.generateSample();
      for (unsigned j = 0; j < outputChannels; j += 1) {//for every sample in frame
        float currentSample = resultOne + resultTwo + resultThree;
        currentSample += blit.generateSample();
        //currentSample = reverb.process(currentSample);
        out[outputChannels * i + j] = currentSample;
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
    // Add your GUI elements here
    pdlAddSlider(app, 0, "Sine", 0.0f, 1.0f, 0.1f);
    pdlAddSlider(app, 1, "Saw", 0.0f, 1.0f, 0.05f);
    pdlAddSlider(app, 2, "Triangle", 0.0f, 1.0f, 0.1f);
    pdlAddSlider(app, 3, "Square", 0.0f, 1.0f, 0.0f);
    pdlAddSlider(app, 4, "DutyCycle", 0.0f, 1.0f, 0.5f);

    pdlAddSlider(app, 5, "Attack", 2.0f, 1000.0f, 80.0f);
    pdlAddSlider(app, 6, "Decay", 2.0f, 1000.0f, 30.0f);
    pdlAddSlider(app, 7, "Sustain", 0.00f, 1.0f, 0.7f);
    pdlAddSlider(app, 8, "Release", 5.0f, 5000.0f, 200.0f);
    pdlAddSlider(app, 9, "Frequency", 1.0, 2000.0f, 100.0f);
    
    pdlAddSlider(app, 10, "dryWetMix", 0.0f, 1.0f, 0.8);

    pdlAddToggle(app, 0, "ADSR_LINEAR", false);
    pdlAddToggle(app, 1, "ADSR_CURVEY", false);
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
