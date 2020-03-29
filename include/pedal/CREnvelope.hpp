#ifndef CREnvelope_hpp
#define CREnvelope_hpp

#include <cmath>
#include "pedal/pdlSettings.hpp"
#include "pedal/utilities.hpp"

#include "pedal/DebugTool.hpp"

//Make a container for segments
struct CurvedSegment{
  float timeInMS;
  float curveCoefficient;
  float curveOffset;
  float timeInSamples;
  float curveOvershoot;
  float offset;
};

class CREnvelope{
  public:
  CREnvelope();
  float generateSample();
  
  enum class modes{
    ADSR, 
    AHDSR, 
    AR, 
    AHR
  };

  void setTrigger(bool newTrigger);
  void setMode(modes newMode);
  void setAttackTime(float newAttackTime);
  void setDecayTime(float newDecayTime);
  void setSustainLevel(float newSustainLevel);
  void setReleaseTime(float newReleaseTime);
  void setHoldTime(float newHoldTime);//applicable to AHDSR and AHR modes
  void setLegato(float newLegato);//
  float getSample();
  modes getCurrentMode();
  float getAttackTime();
  float getDecayTime();
  float getSustainLevel();
  float getReleaseTime();
  bool getTrigger();
  bool isBusy();

  private:
  float currentSample;
  enum states {OFF, ATTACK, DECAY, SUSTAIN, RELEASE, HOLD};
  inline void calculateAttackCurve(float newAttackTime);
  inline void calculateDecayCurve(float newDecayTime);
  inline void calculateReleaseCurve(float newReleaseTime);
  int currentState;
  modes currentMode;
  bool trigger;
  CurvedSegment attack;
  CurvedSegment decay;
  float sustainLevel;
  CurvedSegment release;
  int holdTimeInSamples;
  float holdTimeInMS;
  int holdSampleCount;
  DebugTool debugTool;
};

inline void CREnvelope::calculateAttackCurve(float newAttackTime){
  attack.timeInSamples = pdlSettings::sampleRate * newAttackTime * 0.001f;
  attack.curveCoefficient = std::exp(attack.curveOffset/
                                    attack.timeInSamples);
  attack.offset = (1.0f + attack.curveOvershoot) * 
                 (1.0f - attack.curveCoefficient);
}
inline void CREnvelope::calculateDecayCurve(float newDecayTime){
  decay.timeInSamples = pdlSettings::sampleRate * newDecayTime * 0.001f;
  decay.curveCoefficient = std::exp(decay.curveOffset/ 
                                   decay.timeInSamples);
  decay.offset = (sustainLevel - decay.curveOvershoot) *
                (1.0f - decay.curveCoefficient);
}
inline void CREnvelope::calculateReleaseCurve(float newReleaseTime){
  release.timeInSamples = pdlSettings::sampleRate * newReleaseTime * 0.001f;
  release.curveCoefficient = std::exp(release.curveOffset/
                                     release.timeInSamples);
  release.offset = -release.curveOvershoot * (1.0f - release.curveCoefficient);
}
#endif