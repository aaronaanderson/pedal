/*
// A simple monophonic synth. Monophonic synths, or synths that play
// one note at a time, offer a glimpse at a synthesizer with significantly
// reduced complexity. 

// A midi keyboard can be used to play this synth. For convenience, the ascii
// keyboard will supplement as input. (a through k an the keyboard = C3 to C4)

// A single wavetable Square Wave oscillator is filtered by a resonant lowpass filter
// There are two envelopes: an Amplitude envelope and a filter frequency envelope. These
// envelopes have slightly different timings. The filter frequency envelope's output is scaled
// based on the 'envelope range' parameter

// Aaron Anderson, Oct 2020
*/

#include "example_app.hpp"
#include <iostream>

#include "pedal/MIDIEvent.hpp"

#include "pedal/CREnvelope.hpp"
#include "pedal/WTSquare.hpp"
#include "pedal/Biquad.hpp"
#include "pedal/utilities.hpp"

WTSquare squareOscillator;
CREnvelope amplitudeEnvelope;
Biquad resonantLowPass;
CREnvelope filterEnvelope;

SmoothValue<float> frequency;
SmoothValue<float> amplitude;

int mostRecentNoteOn = -1;

void midiCallback(double deltaTime, std::vector<unsigned char>* message, PedalExampleApp* app){
    MIDIEvent event(message);
    switch(event.getEventType()){
        case MIDIEvent::EventTypes::NOTE_ON:
            mostRecentNoteOn = event.getNoteNumber();
            frequency.setTarget(mtof(event.getNoteNumber()));
            amplitudeEnvelope.setTrigger(true);
            filterEnvelope.setTrigger(true);
            amplitude.setTarget(dBToAmplitude((127 - event.getNoteVelocity()) * -0.1f));
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
            }
        }
    }
}

void audioCallback(float* output, float* input, int bufferSize, int outputChannels, int inputChannels, PedalExampleApp* app){
    //collect these values once per buffer (very lazy form of control rate)
    float portamento = pdlGetSlider(app, 0);
    frequency.setTime(portamento);//set the time it takes to arrive at target value
    float filterRange = pdlGetSlider(app, 1);//scalar for filter frequency envelope outpout
    float outputGainScalar = dBToAmplitude(pdlGetSlider(app, 2));// collect the 'volume' scalar (amplitude scalar)
    for(int sampleIndex = 0; sampleIndex < bufferSize; sampleIndex++){//for every sample
        //update the squareOscillator frequency per sample
        squareOscillator.setFrequency(frequency.process());//update and output the frequency value in one line is clear and concise
        
        //The initial sample will be sourced from the squareOscillator generator, then scaled by the amplitude envelope
        float currentSample = squareOscillator.generateSample() * amplitudeEnvelope.generateSample() * outputGainScalar;
        
        //The filter frequency will be at least as high as the squareOscillator frequency, but will scale via the filter envelope
        float filterFrequency = frequency.getCurrentValue() * (filterEnvelope.generateSample() * filterRange + 1.0f);
        //the filter frequency will have to be set per sample since it modulates
        resonantLowPass.setFrequency(filterFrequency);
        //replace currentSample with a filtered currentSample
        currentSample = resonantLowPass.processSample(currentSample);
        //scale by amplitude (amplitude decided by midiNoteVelocity)
        currentSample *= amplitude.process();
        //assign the sample to every output channel
        for(int channelIndex = 0; channelIndex < outputChannels; channelIndex++){
            output[sampleIndex * outputChannels + channelIndex] = currentSample;
        }
    }
}

int main(){
    //Create the application. pdlInitializeExampleApp requries an audioCallback,
    // but the samplerate and buffersize will default if not provided
    PedalExampleApp* app = pdlInitializeExampleApp(audioCallback, pdlSettings::sampleRate, pdlSettings::bufferSize);
    //optionally, add a qewrty (ascii) keyboard callback
    pdlSetKeyboardCallback(keyboardCallback);
    //optionally, add a MIDI input callback
    pdlSetMidiCallback(app, midiCallback);
    //specify a MIDI port (the devices will print to the terminal, you may have to select a different index)
    pdlOpenMidiPort(app, 2);//<<<<=======================MIDI Port selected Here=============================
    
    //new amplitude targets will arrive smoothly over 100ms
    amplitude.setTarget(100.0f);
    amplitude.setTarget(1.0f);
    //new frequency targets will arrive smoothly over 'portamento' ms
    frequency.setTarget(440.0f);
    frequency.setTime(600.0f);
    
    //I'm going to set these and forget them (but please, explore changing these settings!)
    //This is just the final output amplitude envelope
    amplitudeEnvelope.setMode(CREnvelope::modes::ADSR);
    amplitudeEnvelope.setAttackTime(100.0f);
    amplitudeEnvelope.setDecayTime(40.0f);
    amplitudeEnvelope.setSustainLevel(0.7f);
    amplitudeEnvelope.setReleaseTime(2000.0f);
    //A second envelope will control the filter frequency. This gives each note a bit more variety.
    filterEnvelope.setMode(CREnvelope::modes::ADSR);
    filterEnvelope.setAttackTime(250.0f);
    filterEnvelope.setDecayTime(500.0f);
    filterEnvelope.setSustainLevel(0.5f);
    filterEnvelope.setReleaseTime(1000.0f);
    //I'm setting these and forgetting them as well. 
    resonantLowPass.setMode(Biquad::modes::LOW_PASS);
    resonantLowPass.setFrequency(880.0f);
    resonantLowPass.setQ(4.0f);
    resonantLowPass.setGain(3.0f);

    //add a few sliders for control
    pdlAddSlider(app, 0, "Portamento (ms)", 0.0f, 2000.0f, 250.0f);
    pdlAddSlider(app, 1, "Filter Range", 0.0f, 8.0f, 4.0f);
    pdlAddSlider(app, 2, "Output Gain (dB)", -60.0f, 0.0f, -10.0f);
    
    pdlStartExampleApp(app);//start the app
    while(pdlRunExampleApp(app)){//while the app shouldn't quit
        pdlUpdateExampleApp(app);//update the app
    }
    //if the previous while loop has ended, it is time to close the app
    pdlDeleteExampleApp(app);
}