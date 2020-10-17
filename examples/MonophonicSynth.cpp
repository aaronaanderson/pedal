/*
// A simple monophonic synth. Monophonic synths, or synths that play
// one note at a time, offer a glimpse at a synthesizer with significantly
// reduced complexity. 

// A midi keyboard can be used to play this synth. For convenience, the ascii
// keyboard will supplement as input.
*/

#include "example_app.hpp"
#include <iostream>

#include "pedal/MIDIEvent.hpp"

#include "pedal/CREnvelope.hpp"
#include "pedal/WTSquare.hpp"
#include "pedal/Biquad.hpp"
#include "pedal/utilities.hpp"

WTSquare sawtooth;
CREnvelope amplitudeEnvelope;
Biquad resonantLowPass;
CREnvelope filterEnvelope;

SmoothValue<float> frequency;

int mostRecentNoteOn = -1;
int mostRecentNoteOff = -1;

void midiCallback(double deltaTime, std::vector<unsigned char>* message, PedalExampleApp* app){
    MIDIEvent event(message);
    switch(event.getEventType()){
        case MIDIEvent::EventTypes::NOTE_ON:
            mostRecentNoteOn = event.getNoteNumber();
            frequency.setTarget(mtof(event.getNoteNumber()));
            amplitudeEnvelope.setTrigger(true);
            filterEnvelope.setTrigger(true);
        break;
        case MIDIEvent::EventTypes::NOTE_OFF:
            //Only pay attention to note offs if it belongs to the most recent note
            if(event.getNoteNumber() == mostRecentNoteOn){
                amplitudeEnvelope.setTrigger(false);
                filterEnvelope.setTrigger(false);
            }
        break;
  }
}

void keyboardCallback(int key, bool keyDown){
    int midiNoteNumber = pdlAsciiToMidi(key);//returns -1 if key invalid
    if(midiNoteNumber > -1){//if key mapped to midi note
        if(keyDown){
            mostRecentNoteOn = midiNoteNumber;
            frequency.setTarget(mtof(midiNoteNumber));
            amplitudeEnvelope.setTrigger(true);
            filterEnvelope.setTrigger(true); 
        }else{//key up
            if(midiNoteNumber == mostRecentNoteOn){
                amplitudeEnvelope.setTrigger(false);
                filterEnvelope.setTrigger(false);
                mostRecentNoteOff = midiNoteNumber;
            }
        }
    }
}

void audioCallback(float* output, float* input, int bufferSize, int outputChannels, int inputChannels, PedalExampleApp* app){
    for(int sampleIndex = 0; sampleIndex < bufferSize; sampleIndex++){
        sawtooth.setFrequency(frequency.process());
        float currentSample = sawtooth.generateSample() * amplitudeEnvelope.generateSample() * 0.1f;
        
        //the filter frequency will scale from 1 to 2 octaves above 
        float filterFrequency = frequency.getCurrentValue() * (filterEnvelope.generateSample() * 8.0f + 1.0f);
        resonantLowPass.setFrequency(filterFrequency);
        currentSample = resonantLowPass.processSample(currentSample);
        for(int channelIndex = 0; channelIndex < outputChannels; channelIndex++){
            output[sampleIndex * outputChannels + channelIndex] = currentSample;
        }
    }
}

int main(){
    PedalExampleApp* app = pdlInitializeExampleApp(audioCallback, 48000, 1024);
    pdlSetKeyboardCallback(keyboardCallback);
    pdlSetMidiCallback(app, midiCallback);
    pdlOpenMidiPort(app, 2);
    
    frequency.setTarget(440.0f);
    frequency.setTime(600.0f);
    
    amplitudeEnvelope.setMode(CREnvelope::modes::ADSR);
    amplitudeEnvelope.setAttackTime(100.0f);
    amplitudeEnvelope.setDecayTime(40.0f);
    amplitudeEnvelope.setSustainLevel(0.7f);
    amplitudeEnvelope.setReleaseTime(2000.0f);
    
    filterEnvelope.setMode(CREnvelope::modes::ADSR);
    filterEnvelope.setAttackTime(250.0f);
    filterEnvelope.setDecayTime(500.0f);
    filterEnvelope.setSustainLevel(0.5f);
    filterEnvelope.setReleaseTime(1000.0f);

    resonantLowPass.setMode(Biquad::modes::LOW_PASS);
    resonantLowPass.setFrequency(880.0f);
    resonantLowPass.setQ(4.0f);
    resonantLowPass.setGain(3.0f);

    pdlAddSlider(app, 0, "Portamento", 0.0f, 2000.0f, 250.0f);


    pdlSettings::sampleRate = pdlGetSampleRate(app);
    pdlSettings::bufferSize = pdlGetBufferSize(app);
    pdlStartExampleApp(app);
    while(pdlRunExampleApp(app)){
        pdlUpdateExampleApp(app);
    }
    pdlDeleteExampleApp(app);
}