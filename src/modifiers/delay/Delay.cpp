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
  delete[] currentBlock;
}

float Delay::insertSample(float inputSample){//returns current sample for convenience
  float delayedSample = buffer.getDelayed(delayTime);
  currentSample = inputSample + delayedSample;//sum the delayed and incoming sample
  buffer.inputSample(inputSample + (currentSample * feedback));//feed the circular buffer
  return currentSample;
}

float* Delay::insertBlock(float* inputBlock){
  if(currentBlock == nullptr){
      currentBlock = new float[pdlSettings::bufferSize];
  }
  for(int i = 0; i < pdlSettings::bufferSize; i++){
    //insertSample() returns current sample for convenience
    currentBlock[i] = insertSample(inputBlock[i]);
  }
  return currentBlock;
}

void Delay::setDelayTime(float newDelayTime){
    clamp(delayTime, 0.0f, buffer.getDuration());
}
void Delay::setFeedback(float newFeedback){feedback = newFeedback;}
void Delay::setMaximumFeedbackTime(float newMaximumFeedbackTime){
    buffer.setDuration(newMaximumFeedbackTime);
}
float Delay::getSample(){return currentSample;}
float* Delay::getBlock(){return currentBlock;}
