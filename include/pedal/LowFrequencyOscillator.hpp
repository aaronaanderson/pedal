#ifndef LowFrequencyOscillator_hpp
#define LowFrequencyOscillator_hpp

#include <variant>
#include "pedal/utilities.hpp"
#include "pedal/TrivialSine.hpp"
#include "pedal/TrivialTriangle.hpp"
#include "pedal/TrivialSaw.hpp"
#include "pedal/TrivialSquare.hpp"

namespace pedal{
class LowFrequencyOscillator{
  public:
  LowFrequencyOscillator();
  ~LowFrequencyOscillator();
  
  float generateSample();
  
  void setFrequency(float newFrequency);
  void setPhase(float newPhase);
  void setAmplitude(float newAmplitude);
  void flipWaveform(bool shouldFlipWaveform);
  enum class WaveShape{
    Sine =0, 
    Triangle, 
    Saw, 
    Square,
  };
  void setWaveShape(WaveShape newWaveShape);
  void setOutputRange(float outputMin, float outputMax);  
  float getSample();
  float getFrequency();
  double getPhase();
  float getAmplitude();
  bool getShouldFlipWaveform();
  WaveShape getWaveShape();
  float getOutputRangeLowerBound();
  float getOutputRangeUpperBound();  
  private:
  std::variant<TrivialSine*, TrivialTriangle*, TrivialSaw*, TrivialSquare*> oscillator;
  WaveShape currentWaveShape;
  float currentSample;
  float frequency;
  double phase;
  float amplitude;
  bool shouldFlipWaveform;
  float outputRangeLowerBound;
  float outputRangeUpperBound;
  void deletePreviousOscillator(WaveShape);
};
}

#endif

    // union Oscillator{
    //     TrivialSine sine;
    //     TrivialTriangle triangle;
    //     TrivialSaw saw;
    //     TrivialSquare square;
    //     TPhasor phasor;
    //     Oscillator(){new(&sine)TrivialSine(440.0f);}
    //     ~Oscillator(){delete(&sine);}
    // };
    // Oscillator unionOscillator;