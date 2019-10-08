#include "pedal/HanningWindow.hpp"

//Constructors and Deconstructors=============
HanningWindow::HanningWindow(){
  setDuration(1000.0f);
  trigger = false;
}
HanningWindow::HanningWindow(float initialDuration){
  setDuration(initialDuration);
  trigger = false;
}
HanningWindow::~HanningWindow(){
  delete[] currentBlock;
}
//Core functionality===========================
float HanningWindow::generateSample(){
  if(active){
    currentSample = sampleFromPhase(phase);
    phase += phaseIncrement;
    if(phase > 1.0f){
      phase = 0.0f;
      active = false;
    }
  } 
  return currentSample;
}
float* HanningWindow::generateBlock(){
  if(currentBlock == nullptr){
    currentBlock = new float[pdlSettings::bufferSize];
  }
  for(int i = 0; i < pdlSettings::bufferSize; i++){
    currentBlock[i] = generateSample();
  }
  return currentBlock;
}

//Getters and Setters===============================
void HanningWindow::setTrigger(bool newTrigger){
  if(trigger == false && newTrigger == true){//if it was off but now will be on
    phase = 0.0;//reset
    active = true;
  }
  trigger = newTrigger;//update the trigger status
}
void HanningWindow::setDuration(float newDuration){
  duration = newDuration;
  phaseIncrement = 1.0f/(pdlSettings::sampleRate*(duration*0.001));
}
void HanningWindow::setPhase(float newPhase){phase = newPhase;}

float HanningWindow::getCurrentSample(){return currentSample;}
float* HanningWindow::getCurrentBlock(){return currentBlock;}
float HanningWindow::getDuration(){return duration;}
float HanningWindow::getPhase(){return phase;}