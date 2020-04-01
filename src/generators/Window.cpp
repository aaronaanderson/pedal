#include "pedal/Window.hpp"

//Constructors and Deconstructors=============
Window::Window(){
  setDuration(1000.0f);
  trigger = false;
}
Window::Window(float initialDuration){
  setDuration(initialDuration);
  trigger = false;
}

//Core functionality===========================
float Window::generateSample(){
  if(active){
    switch(currentMode){
      case Mode::HANNING:
        currentSample = hanningFromPhase(phase);
      break;
      case Mode::HAMMING:
        currentSample = hammingFromPhase(phase);
      break;
      case Mode::COSINE:
        currentSample = cosineFromPhase(phase);
      break;
      case Mode::TRIANGULAR:
        currentSample = triangularFromPhase(phase);
      break;
      case Mode::BLACKMAN_NUTALL:
        currentSample = blackmanNutallFromPhase(phase);
      break;
      case Mode::GAUSSIAN:
        currentSample = gaussianFromPhase(phase);
      break;
    }
    phase += phaseIncrement;
    if(phase > 1.0f){
      phase = 0.0f;
      active = false;
    }
  }else{//if window is not active
    currentSample = 0.0f;
  }
  return currentSample;
}

//Getters and Setters===============================
void Window::setTrigger(bool newTrigger){
  if(trigger == false && newTrigger == true){//if it was off but now will be on
    phase = 0.0;//reset
    active = true;
  }
  trigger = newTrigger;//update the trigger status
}
void Window::setDuration(float newDuration){
  duration = newDuration;
  phaseIncrement = 1.0f/(pdlSettings::sampleRate*(duration*0.001));
}
void Window::setPhase(float newPhase){
  phase = clamp(newPhase, 0.0f, 1.0f);
}
float Window::getCurrentSample(){return currentSample;}
float Window::getDuration(){return duration;}
float Window::getPhase(){return phase;}
bool Window::getActive(){return active;}