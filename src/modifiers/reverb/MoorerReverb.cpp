#include "pedal/MoorerReverb.hpp"
MoorerReverb::MoorerReverb(){
  //set Low Pass Comb Filters
  for(int i = 0; i < NUM_COMBS; i++){
    filterBank[i].setMaxDelayTime(100.0f);
    filterBank[i].setDelayTime(combDelayTimess[i]);
    filterBank[i].setFilterFrequency(combFilterFrequencies[i]);
  }
  //set All Pass filter
  allPass.setMaxDelayTime(10.0f);//save memory by setting this as low as possible(but as high as needed)
  allPass.setDelayTime(6.0f);
  allPass.setCoefficient(0.7f);
  //set dryWet (0.0 is no reverb)
  dryWetMix = 1.0f;
}

//============================================
float MoorerReverb::process(float input){
  currentSample = 0.0f;
  delayLine.inputSample(input);//feed the delay line
  //handle th early reflections
  for(int i = 0; i < NUM_TAPS; i++){//for every tap (of multitap delay)
    currentSample += delayLine.getDelayed(delayTimes[i]) * //check array for time
                     delayGains[i];//scale buffer read by gain
  }
  //feed every comb filter the same input (because parallel)
  float filterBankSum = 0.0f;//save cpu cycles by collecting this as we feed the filters
  for(int i = 0; i < NUM_COMBS; i++){//for every comb filter
    filterBankSum += filterBank[i].process(currentSample);//each filter gets the same input
  }
  //next is allpass
  currentSample = allPass.process(filterBankSum);
  currentSample = (currentSample * dryWetMix) + (input * (1.0f-dryWetMix));
  return currentSample;
}