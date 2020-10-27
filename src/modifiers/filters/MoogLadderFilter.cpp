#include "pedal/MoogLadderFilter.hpp"

MoogLadderFilter::MoogLadderFilter(){
    //create the four sub lowpass filters
    for(int i = 0; i < NUM_SUB_FILTERS; i++){
        filterArray.push_back(CompromiseOnePoleFilter());
    }
    setMode(MoogLadderFilter::MODE::FOURPOLE_LOWPASS);
    setFrequency(800.0f);
    setResonance(1.0f - (1.0f/800.0f));
}
void MoogLadderFilter::setMode(MoogLadderFilter::MODE newMode){
    currentMode = newMode;
    setModeScalarsFromTable();
}
void MoogLadderFilter::setFrequency(float newFrequency){
    normalizedAngularFrequency = (newFrequency * 2.0f * M_PI) / pdlSettings::sampleRate;
    float g = (0.9892f * normalizedAngularFrequency) - 
              std::pow(0.4342 * normalizedAngularFrequency, 2) + 
              std::pow(0.1381 * normalizedAngularFrequency, 3) - 
              std::pow(0.0202 * normalizedAngularFrequency, 4);
    for(int i = 0; i < NUM_SUB_FILTERS; i++){
        filterArray[i].setG(g);
    }
}
void MoogLadderFilter::setResonance(float newResonance){
    float input = clamp(newResonance, 0.0f, 1.0f);
    resonance = input * (1.0029 + 
                        (0.0526 * normalizedAngularFrequency) - 
                        std::pow(0.0926 * normalizedAngularFrequency, 2) + 
                        std::pow(0.0218 * normalizedAngularFrequency, 3));
}
void MoogLadderFilter::setModeScalarsFromTable(){
    int modeIndex = (int)currentMode;
    for(int i = 0; i < 5; i++){
        modeScalars[i] = modeTable[modeIndex][i];
    }
}