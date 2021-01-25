#include "pedal/NSegmentLinearEnvelope.hpp"

using namespace pedal;

NSegmentLinearEnvelope::NSegmentLinearEnvelope(){
  setInitialValue(0.0f);//sets the target value of the 0th segment index
  currentSample = initialValue;
  currentSegment = 0;
}

float NSegmentLinearEnvelope::generateSample(){
  if(active){
    currentSample += segments[currentSegment].increment;
    //check to see if it is time to move on to the next segment
    if(std::signbit(segments[currentSegment].increment)){//if increment is negative
      if(currentSample < segments[currentSegment].targetValue){incrementSegment();}
    }else{//if increment is positive
      if(currentSample > segments[currentSegment].targetValue){incrementSegment();}
    }
  }else{
    currentSample = 0.0f;
  }
  return currentSample;
} 

void NSegmentLinearEnvelope::appendSegment(float targetValue, float durationMS){
  //this value is true to get the time from 0.0 to 1.0
  double increment = 1.0f / (Settings::sampleRate * durationMS * 0.001f);
  //scale the increment to adjust for starting point
  float previousTargetValue;
  if(segments.size() > 0){
    previousTargetValue = segments[segments.size()-1].targetValue;  
  }else{
    previousTargetValue = initialValue;
  }
  increment *= 1.0f/(targetValue - previousTargetValue);
  segments.push_back({targetValue, increment, durationMS});
}
void NSegmentLinearEnvelope::insertSegment(const int insertIndex, float targetValue, float durationMS){
  //this value is true to get the time from 0.0 to 1.0
  //scale the increment to adjust for starting point
  float prevoiusTargetValue;
  if(insertIndex > 0){
    prevoiusTargetValue = segments[insertIndex-1].targetValue;
  }else{
    prevoiusTargetValue = initialValue;
  }
  double increment = 1.0f / (Settings::sampleRate * durationMS * 0.001f * (targetValue - prevoiusTargetValue));
  segments.insert(segments.begin() + insertIndex, {targetValue, increment, durationMS});
  //Make sure to addjust the increment of the next segment, since now it may have a different starting value
  if(insertIndex < segments.size()){
    const int nextIndex = insertIndex + 1;
    segments[nextIndex].increment = 1.0f/(Settings::sampleRate * segments[nextIndex].durationMS * 0.001f * (segments[nextIndex].targetValue - segments[insertIndex].targetValue));
  } 
}
void NSegmentLinearEnvelope::moveSegment(int originatingIndex, int destinationIndex){
    //make a temporary copy
    Segment originalSegment = segments[originatingIndex];
    //delete the array copy
    segments.erase(segments.begin() + originatingIndex);
    //insert the temporary copy
    insertSegment(destinationIndex, originalSegment.targetValue, originalSegment.durationMS);
}
void NSegmentLinearEnvelope::removeSegment(int segmentIndex){
  if(segmentIndex < segments.size()){
    segments.erase(segments.begin() + segmentIndex);
  }
}
void NSegmentLinearEnvelope::clearSegments(){
    segments.clear();
}
void NSegmentLinearEnvelope::setTrigger(bool newTrigger){
  if(newTrigger == true && trigger == false){
    currentSegment = 0;
    active = true;
  }
  trigger = false;
}
void NSegmentLinearEnvelope::setInitialValue(float initialValue){

}
bool NSegmentLinearEnvelope::getTrigger(){return trigger;}
float NSegmentLinearEnvelope::getSample(){return currentSample;}
void NSegmentLinearEnvelope::incrementSegment(){
  currentSegment++;
  if(currentSegment >= segments.size()){
    active = false;
    currentSegment = 0;
  }
}