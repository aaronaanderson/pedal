#ifndef CREnvelope_hpp
#define CREnvelope_hpp

class CREnvelope{
  public:
  CREnvelope();
  float generateSample();
  
  void setTrigger(bool newTrigger);
  void setMode(EnvelopeModes newMode);
  void setAttackTime(float newAttackTime);
  void setDecayTime(float newDecayTime);
  void setSustainLevel(float newSustainLevel);
  void setReleaseTime(float newReleaseTime);

  float getSample();
  int getState();
  EnvelopeModes getCurrentMode();
  float getAttackTime();
  float getDecayTime();
  float getSustainLevel();
  float getReleaseTime();
  bool getTrigger();
  bool isBusy();
  private:
  float currentSample;
  enum states {OFF = 0, ATTACK, DECAY, SUSTAIN, RELEASE};
  int currentState;
  int currentMode;
  bool trigger;
  float attackTime;
  float attackCurveCoefficient;
  float attackCurveOffset;
  float decayTime;
  float decayCurveCoefficient;
  float decayCurveOffset;
  float sustainLevel;
  float releaseTime;
  float releaseCurveOffset;
  float releaseCurveCoefficient;
};
#endif