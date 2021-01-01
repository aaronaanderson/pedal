#ifndef EdgeDetector_hpp
#define EdgeDetector_hpp

#include <cmath>
//returns true if edge detected
//useful for using a phasor as a timer
namespace pedal{
class EdgeDetector{
  public:
  EdgeDetector();

  bool process(float input);
  void allowRisingEdge(bool allowRisingEdge);
  void allowFallingEdge(bool allowFallingEdge);
  void setRisingThreshold(float newRisingThreshold);
  void setFallingThreshold(float newFallingThreshold);
  float getLastFallingEdge();
  float getLastRisingEdge();

  private:
  bool currentEdge;
  float previousInput;
  float risingThreshold;
  float fallingThreshold;
  float currentRisingEdge;
  float currentFallingEdge;
  bool risingEdgeFlag;
  bool fallingEdgeFlag;
};
}//end pedal namespace
#endif