#include "example_app.hpp"
#include <iostream>

#include <vector>
#include "pedal/WTSaw.hpp"
#include "pedal/MoogLadderFilter.hpp"
#include "pedal/utilities.hpp"
#include "pedal/Counter.hpp"
#include "pedal/TPhasor.hpp"
#include "pedal/EdgeDetector.hpp"
#include "pedal/CTEnvelope.hpp"
using namespace pedal;

MoogLadderFilter filter;
SmoothValue<float> frequency;
Counter counter(8);
TPhasor timer(32.0f);
EdgeDetector edgeDetector;

std::vector<int> sequence = {40, 42, 44, 45, 47, 49, 51, 52};
WTSaw saw;
CTEnvelope envelope;
void keyboardCallback(int key, bool keyDown){

}

void audioCallback(float* output, float* input, int bufferSize, int inputChannels, int outputChannels, PedalExampleApp* app){


  for(int sampleIndex = 0; sampleIndex < bufferSize; sampleIndex++){
    //check for an envelope trigger
    if(edgeDetector.process(timer.generateSample())){
      frequency.setTarget(mtof(sequence[counter.getCount()]));
      counter.increment();
      std::cout << counter.getCount() << std::endl;
      envelope.setTrigger(true);
    }
    saw.setFrequency(frequency.process());
    float currentSample = saw.generateSample() * envelope.generateSample();

    for(int channelIndex = 0; channelIndex < outputChannels; channelIndex++){
      output[sampleIndex * outputChannels + channelIndex] = currentSample * 0.1f;
    }
  }
}

int main(){
  //Create the application (an audio callback is required here)
  PedalExampleApp* app = pdlInitializeExampleApp(audioCallback, pdlSettings::sampleRate, pdlSettings::bufferSize);
  pdlSetKeyboardCallback(keyboardCallback);
  saw.setFrequency(30.0f);
  envelope.setMode(CTEnvelope::Mode::AR);
  envelope.setAttackTime(35.0f);
  envelope.setDecayTime(300.0f);
  filter.setResonance(0.94f);

  pdlStartExampleApp(app);
  //This is the perpetual loop; it will keep going until the window is closed
  while(pdlRunExampleApp(app)){//while the window is still open
    pdlUpdateExampleApp(app);//continue running the app
  }
  //If this point is reached, the application is closing
  pdlDeleteExampleApp(app);
}