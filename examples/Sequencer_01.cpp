#include "example_app.hpp"
#include <iostream>

#include <vector>
#include "pedal/WTrivialSaw.hpp"
#include "pedal/MoogLadderFilter.hpp"
#include "pedal/utilities.hpp"
#include "pedal/Counter.hpp"
#include "pedal/Phasor.hpp"
#include "pedal/EdgeDetector.hpp"
#include "pedal/CurvedEnvelope.hpp"

using namespace pedal;

SmoothValue<float> frequency;

Phasor timer(bpmToHz(60.0));
EdgeDetector edgeDetector;

std::vector<int> sequence = {40, 42, 44, 45, 47, 49, 51, 52};
Counter counter(sequence.size());
WTrivialSaw saw;
CurvedEnvelope envelope;

SmoothValue<float> outputVolume;

void audioCallback(float* output, float* input, int bufferSize, int inputChannels, int outputChannels, app::PedalExampleApp* app){
  timer.setFrequency(bpmToHz(app::getSlider(app, 0)));
  int sequenceOffset = app::getSlider(app, 1);
  outputVolume.setTarget(dBToAmplitude(app::getSlider(app, 2)));

  for(int sampleIndex = 0; sampleIndex < bufferSize; sampleIndex++){
    //Timer will generate N edged per second, which will be deteced by the edgeDetector
    if(edgeDetector.process(timer.generateSample())){
      frequency.setTarget(mtof(sequence[counter.getCount()] + sequenceOffset));
      counter.increment();
      envelope.setTrigger(true);
    }
    saw.setFrequency(frequency.process());
    float currentSample = saw.generateSample() * envelope.generateSample();

    for(int channelIndex = 0; channelIndex < outputChannels; channelIndex++){
      output[sampleIndex * outputChannels + channelIndex] = currentSample * outputVolume.process();
    }
  }
}

int main(){
  //Create the application (an audio callback is required here)
  app::PedalExampleApp* app = app::pdlInitializeExampleApp(audioCallback, Settings::sampleRate, Settings::bufferSize);
  saw.setFrequency(30.0f);
  envelope.setMode(CurvedEnvelope::Mode::AR);
  envelope.setAttackTime(35.0f);
  envelope.setDecayTime(150.0f);
  
  app::addSlider(app, 0, "BPM", 30.0f, 600.0f, 145.0f);
  app::addSlider(app, 1, "Sequence offset", - 12, 12, 0);
  app::addSlider(app, 2, "Output Gain(dB)", -60, 0.0f, -3.0f);
  app::startApp(app);
  //This is the perpetual loop; it will keep going until the window is closed
  while(app::shouldContinue(app)){//while the window is still open
    app::update(app);//continue running the app
  }
  //If this point is reached, the application is closing
  app::freeMemory(app);
}