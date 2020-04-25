#include "pedal/EdgeDetector.hpp"
EdgeDetector::EdgeDetector(){
  allowRisingEdge(true);
  allowFallingEdge(true);
  setRisingThreshold(0.5f);
  setFallingThreshold(0.5f);
  currentFallingEdge = 0.0f;
  currentRisingEdge = 0.0f;
}
bool EdgeDetector::process(float input){
  float delta = std::fabs(input - previousInput);
  if(risingEdgeFlag){
    if(delta > risingThreshold){
      currentEdge = true;
      currentRisingEdge = delta;
    }else
    {
      currentEdge = false;
    }
  }
  if(fallingEdgeFlag){
    if(delta > fallingThreshold){
      currentEdge = true;
      currentFallingEdge = delta;
    }else{
      currentEdge = false;
    }
  }
  previousInput = input;
  return currentEdge;
}
void EdgeDetector::allowRisingEdge(bool allowRisingEdge){risingEdgeFlag = allowRisingEdge;}
void EdgeDetector::allowFallingEdge(bool allowFallingEdge){fallingEdgeFlag = allowFallingEdge;}
void EdgeDetector::setRisingThreshold(float newRisingThreshold){risingThreshold = newRisingThreshold;}
void EdgeDetector::setFallingThreshold(float newFallingThreshold){fallingThreshold = newFallingThreshold;}
float EdgeDetector::getLastFallingEdge(){return currentFallingEdge;}
float EdgeDetector::getLastRisingEdge(){return currentRisingEdge;}
