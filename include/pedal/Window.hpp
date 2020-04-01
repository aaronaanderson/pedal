#ifndef Window_hpp
#define Window_hpp

#define _USE_MATH_DEFINES
#include <cmath>
#include "utilities.hpp"
#include "pdlSettings.hpp"
/*
Windows are generally used to gradually scale the amplitude of
an array of samples to zero at both ends. The shape of a window
has spectral implications. The correct choice of window function
is dependent upon application. 
*/
class Window {
  public:
  Window();
  Window(float initialDuration);
  float generateSample();
  enum class Mode{
    HANNING, 
    HAMMING, 
    COSINE, 
    TRIANGULAR, 
    BLACKMAN_NUTALL, 
    GAUSSIAN 
  };
  
  void setTrigger(bool newTrigger);
  void setDuration(float newDuration);
  void setPhase(float newPhase);
  void setMode(Window::Mode newMode);
  float getCurrentSample();
  bool getActive();
  float* getCurrentBlock();
  float getDuration();
  float getPhase();
  Window::Mode getMode();

  private:
  bool trigger;
  bool active;
  float duration;
  float phase, phaseIncrement;
  Window::Mode currentMode;
  float currentSample;
  
  //functions used to calculate windows
  //can be used without creating a Window object
  public:
  static inline float hanningFromPhase(float inputPhase){
    return 0.5f * (1.0f - std::cos(2.0f * M_PI * inputPhase));
  }
  static inline float hammingFromPhase(float inputPhase){
    return 0.54 - (0.46 * std::cos(2.0f * M_PI * inputPhase));
  }
  static inline float cosineFromPhase(float inputPhase){
    return std::sin(M_PI * inputPhase);
  }
  static inline float triangularFromPhase(float inputPhase){
    return 1.0f - std::abs(2.0f * inputPhase - 1.0f);
  }
  static inline float blackmanNutallFromPhase(float inputPhase){
    return 0.3635819f - 
       (0.4891775f * std::cos(2.0f * M_PI * inputPhase)) + 
       (0.1365995f * std::cos(4.0f * M_PI * inputPhase)) + 
       (0.0106411f * std::cos(6.0f * M_PI * inputPhase));
  }
  static inline float gaussianFromPhase(float inputPhase, const float kurtosis = 0.3f){
    float gausRecip = 1.0/(-2.0 * kurtosis * kurtosis);
    float phase = inputPhase * 2.0f - 1.0f;
    phase *= phase;
    return std::exp(phase * gausRecip);
  }
};
#endif