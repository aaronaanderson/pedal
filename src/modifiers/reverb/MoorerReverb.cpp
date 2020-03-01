#include "pedal/MoorerReverb.hpp"
/*
James Moorer "About this Reverberation Business" - 1979

This paper discusses modern (to 1979) techniques in digital 
reverb. Naming and conventions are kept identical and clear
to the paper, when possible. This model is a significant
improvement to Schroeder's model, but, while it can produce
useful results, it is include mostly for academic purposes.
*/
//============================================
MoorerReverb::MoorerReverb(){
  g = 0.94f;//set master feedback
  time = 0.366f/(1.0f - g);//estimation, from page 26
  //set Low Pass Comb Filters
  for(int i = 0; i < NUM_COMBS; i++){
    filterBank[i].setCombDelayTime(combDelayTimess[i]);//set times from header
    filterBank[i].setG1(gainArray[i]);//these values are explained in header
    filterBank[i].setG2((1.0f - filterBank[i].getG1()) * g);
  }
  //set All Pass filter
  allPass.setMaxDelayTime(6.0f);//save memory by setting this as low as possible(but as high as needed)
  allPass.setDelayTime(6.0f);//Moorer suggest there isn't much room for variance on this value
  allPass.setCoefficient(0.7f);
  //set dryWet (0.0 is no reverb)
  dryWetMix = 1.0f;//not in original design, but useful
  delayLine.setDuration(80.0f);//set to max delay time to not waste memory
  lagBuffer.setDuration(reverbLagTime);//delayed copy of early reflections
}
//============================================
float MoorerReverb::process(float input){
  currentSample = 0.0f;
  delayLine.inputSample(input);//feed the delay line
  //handle th early reflections
  float earlyReflections = 0.0f;
  for(int i = 0; i < NUM_TAPS; i++){//for every tap (of multitap delay)
    earlyReflections += delayLine.getDelayed(taps[i].time) * //check array for time
                     taps[i].amplitude;//scale buffer read by gain
  }
  lagBuffer.inputSample(earlyReflections + input);//see Fig. 12, essentially pre-delay
  //feed every comb filter the same input (because parallel)
  float filterBankSum = 0.0f;//save cpu cycles by collecting this as we feed the filters
  for(int i = 0; i < NUM_COMBS; i++){//for every comb filter
    filterBankSum += filterBank[i].process(earlyReflections);//each filter gets the same input
  }
  //next is allpass
  currentSample = allPass.process(filterBankSum);
  //add back the delayed, dry early reflections
  lagBuffer.getDelayed(reverbLagTime);//See Fig. 12
  currentSample = (currentSample * dryWetMix) + (input * (1.0f-dryWetMix));
  return currentSample;
}
//============================================
float MoorerReverb::getSample(){return currentSample;}
float MoorerReverb::getG(){return g;}
float MoorerReverb::getReverbTime(){return time;}
float MoorerReverb::getDryWetMix(){return dryWetMix;}
void MoorerReverb::setReverbTime(float newTime){//pg 26 of paper
  //this is an aproximation, but should be within 10%
  //accoring to the paper. Moorer claims that sampling rate
  //shouldn't affect this value significantly, but I find this
  //claim dubious.
  time = newTime;
  MoorerReverb::setG(1.0f - (0.366f/(time*0.001f)));
}
void MoorerReverb::setDryWetMix(float newDryWetMix){dryWetMix = newDryWetMix;}
void MoorerReverb::setG(float newG){//set global feed back
  g = clamp(newG, 0.0f, 1.0f);//>1.0 is unstable
  for(int i = 0; i < NUM_COMBS; i++){//for every comb
    filterBank[i].setG1(gainArray[i]);//these values are explained in header
    filterBank[i].setG2((1.0f - filterBank[i].getG1()) * g);//set comb gain based on lpf gain
  }
}