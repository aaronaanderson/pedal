#include "pedal/Delay.hpp"

Delay::Delay(){
  buffer.setDuration(4000.0f);//allocate 4 seconds worth of samples
  tap = new BufferTap(&buffer);//give the tap a buffer reference
  tap->setDelayTime(250.0f);//read from 1/4 second ago
}
Delay::Delay(float initialTime, float initialFeedback){
  buffer.setDuration(initialTime);//allocate memory
  tap = new BufferTap(&buffer);//give the tap a buffer reference
  tap->setDelayTime(initialTime);//read from initialTime ms ago
}
Delay::~Delay(){
  delete tap;
}
float Delay::insertSample(float inputSample){//returns current sample for convenience
  currentSample = buffer.getDelayed(delayTime);//retrieve the delayed sample
  buffer.inputSample(inputSample + currentSample * feedback);
  return currentSample;
}
void Delay::setDelayTime(float newDelayTime){
    clamp(newDelayTime, 0.0f, buffer.getDuration());//clamp value to useable range
    delayTime = newDelayTime;
}
void Delay::setFeedback(float newFeedback){feedback = newFeedback;}
void Delay::setMaximumFeedbackTime(float newMaximumFeedbackTime){
    //delayTime can only be between 0 and the maximum time
    //this function allocates samples for the maximum delay Time
    buffer.setDuration(newMaximumFeedbackTime);
}
float Delay::getDelayTime(){return delayTime;}
float Delay::getFeedback(){return feedback;}
float Delay::getMaximumFeedbackTime(){return buffer.getDuration();}
float Delay::getSample(){return currentSample;}