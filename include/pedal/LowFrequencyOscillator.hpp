#ifndef LowFrequencyOscillator_hpp
#define LowFrequencyOscillator_hpp

#include <variant>

#include "pedal/TSine.hpp"
#include "pedal/TTriangle.hpp"
#include "pedal/TSaw.hpp"
#include "pedal/TSquare.hpp"
#include "pedal/TPhasor.hpp"

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
        Sine, 
        Triangle, 
        Saw, 
        Square
    };
    void setWaveShape(WaveShape newWaveShape);
    
    float getSample();
    float getFrequency();
    double getPhase();
    float getAmplitude();
    bool getShouldFlipWaveform();
    WaveShape getWaveShape();

    private:
    std::variant<TSine*, 
                 TTriangle*, 
                 TSaw*, 
                 TSquare*> oscillator;
 
    WaveShape currentWaveShape;
    float frequency;
    double phase;
    float amplitude;
    bool shouldFlipWaveform;
};
}

#endif