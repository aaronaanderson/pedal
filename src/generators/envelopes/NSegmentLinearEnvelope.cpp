#include "pedal/NSegmentLinearEnvelope.hpp"

#include <iostream>
using namespace pedal;

NSegmentLinearEnvelope::NSegmentLinearEnvelope(){
  setInitialValue(0.0f);//sets the target value of the 0th segment index
  currentSample = initialValue;
  currentSegment = 0;
  sustainSegment = -1;//-1 indicates no sustain point 
  isSustaining = trigger = false;
}

float NSegmentLinearEnvelope::generateSample(){
  if(active){
    if(!isSustaining){//if not on a sustain segment
      currentSample += segments[currentSegment].increment;
      //check to see if it is time to move on to the next segment
      if(std::signbit(segments[currentSegment].increment)){//if increment is negative
        if(currentSample < segments[currentSegment].targetValue){
          if(currentSegment == sustainSegment){isSustaining=true;}
          else{incrementSegment();}
        }
      }else{//if increment is positive
        if(currentSample > segments[currentSegment].targetValue){
          if(currentSegment == sustainSegment){isSustaining=true;}
          else{incrementSegment();}
        }
      }
    }else{
      currentSegment = segments[currentSegment].targetValue;
    }
  }else{
    currentSample = 0.0f;
  }
  return currentSample;
} 

void NSegmentLinearEnvelope::appendSegment(float targetValue, float durationMS){
  float previousTargetValue;
  if(segments.size() > 0){
    previousTargetValue = segments[segments.size()-1].targetValue;  
  }else{
    previousTargetValue = initialValue;
  }
  double increment = calculateIncrement(targetValue-previousTargetValue, durationMS);
  segments.push_back({targetValue, increment, durationMS});
}
void NSegmentLinearEnvelope::insertSegment(const int insertIndex, float targetValue, float durationMS){
  //this value is true to get the time from 0.0 to 1.0
  //scale the increment to adjust for starting point
  float previousTargetValue;
  if(insertIndex > 0){
    previousTargetValue = segments[insertIndex-1].targetValue;
  }else{
    previousTargetValue = initialValue;
  }
  double increment = calculateIncrement(targetValue-previousTargetValue, durationMS);
  segments.insert(segments.begin() + insertIndex, {targetValue, increment, durationMS});
  //Make sure to addjust the increment of the next segment, since now it may have a different starting value
  if(insertIndex < segments.size()){
    const int nextIndex = insertIndex + 1;
    segments[nextIndex].increment = calculateIncrement(segments[currentSegment].targetValue - segments[nextIndex].targetValue, 
                                                       segments[nextIndex].durationMS);
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
  if(sustainSegment < 0){//If no sustain point
    if(newTrigger == true && trigger == false){
      currentSegment = 0;
      active = true;
      trigger = false;//set for re-trigger
    }
  }else{//if there is a sustain point
    if(newTrigger == true && trigger == false){//if new note-on
      currentSegment = 0;
      active = true;
      std::cout << ".." << std::endl;
    }
    if(newTrigger == false && trigger == true){
      isSustaining = false;
    }
    trigger = newTrigger;
  }
}
void NSegmentLinearEnvelope::setInitialValue(float newInitialValue){initialValue = newInitialValue;}
void NSegmentLinearEnvelope::setSustainSegment(int whichSegment){
  if(whichSegment < segments.size()){
    std::cout << "sustain segment set to " << whichSegment << std::endl;
    sustainSegment = whichSegment;
  }
}
void NSegmentLinearEnvelope::removeSustain(){sustainSegment = -1;}
bool NSegmentLinearEnvelope::getTrigger(){return trigger;}
int NSegmentLinearEnvelope::getSustainSegment(){return sustainSegment;}
int NSegmentLinearEnvelope::getNumberOfSegments(){return segments.size();}
float NSegmentLinearEnvelope::getSample(){return currentSample;}
void NSegmentLinearEnvelope::incrementSegment(){
  currentSegment++;
  //make inactive and reset if end reached
  if(currentSegment >= segments.size()){
    active = false;
    currentSegment = 0;
  }
}
double NSegmentLinearEnvelope::calculateIncrement(float distance, float durationMS){
  double increment;
  if(distance > 0.0f){
    increment = 1.0 / (Settings::sampleRate * 0.001f * durationMS * distance);
  }else{
    increment = 0.0;
  }
  return increment;
}