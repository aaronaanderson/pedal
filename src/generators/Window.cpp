 #include "pedal/Window.hpp"

using namespace pedal;

//Constructors and Deconstructors=============
Window::Window(){
  setDuration(1000.0f);
  setMode(Window::Mode::HANNING);
  trigger = active = false;
  
}
Window::Window(float initialDuration){
  setDuration(initialDuration);
  setMode(Window::Mode::HANNING);
  trigger = active = false;
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
    if(phase > pedal::TWOPI){
      phase = 0.0f;
      active = trigger = false;
    }
  }else{//if window is not active
    currentSample = 0.0f;
  }
  return currentSample;
}
#include <iostream>
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
  phaseIncrement = pedal::TWOPI/(pdlSettings::sampleRate*(duration*0.001));
}
void Window::setMode(Window::Mode newMode){
  currentMode = newMode;
}
void Window::setPhase(float newPhase){
  phase = clamp(newPhase, 0.0f, pedal::TWOPI);
}
float Window::getCurrentSample(){return currentSample;}
float Window::getDuration(){return duration;}
float Window::getPhase(){return phase;}
bool Window::getActive(){return active;}