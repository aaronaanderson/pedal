/*
// IN PROGRESS:
// A simple sound file player with playback speed
// controlled by note value
//
// Aaron Anderson, Oct 2020
*/

#include "example_app.hpp"
#include <iostream>

#include "pedal/MIDIEvent.hpp"
#include "pedal/CREnvelope.hpp"
#include "pedal/utilities.hpp"
#include "pedal/Buffer.hpp"

Buffer soundBuffer;
CREnvelope amplitudeEnvelope;
SmoothValue<float> amplitude;

int mostRecentNoteOn = -1;

void midiCallback(double deltaTime, std::vector<unsigned char>* message, PedalExampleApp* app){
    MIDIEvent event(message);
    switch(event.getEventType()){
        case MIDIEvent::EventTypes::NOTE_ON:
            mostRecentNoteOn = event.getNoteNumber();
            amplitudeEnvelope.setTrigger(true);
            amplitude.setTarget(dBToAmplitude((127 - event.getNoteVelocity()) * -0.1f));
        break;
        case MIDIEvent::EventTypes::NOTE_OFF:
            //Only pay attention to note offs if it belongs to the most recent note
            if(event.getNoteNumber() == mostRecentNoteOn){
                amplitudeEnvelope.setTrigger(false);
            }
        break;
  }
}

void keyboardCallback(int key, bool keyDown){
    int midiNoteNumber = pdlAsciiToMidi(key);//returns -1 if key invalid
    if(midiNoteNumber > -1){//if key mapped to midi note
        if(keyDown){
            mostRecentNoteOn = midiNoteNumber;
            amplitudeEnvelope.setTrigger(true);
        }else{//key up
            if(midiNoteNumber == mostRecentNoteOn){
                amplitudeEnvelope.setTrigger(false);
            }
        }
    }
}

void audioCallback(float* output, float* input, int bufferSize, int outputChannels, int inputChannels, PedalExampleApp* app){
    
    //collect these values once per buffer (very lazy form of control rate)
    float portamento = pdlGetSlider(app, 0);
    float outputGainScalar = dBToAmplitude(pdlGetSlider(app, 2));// collect the 'volume' scalar (amplitude scalar)
    
    for(int sampleIndex = 0; sampleIndex < bufferSize; sampleIndex++){//for every sample
        float currentSample = 0.0f;



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
    
    std::string filePath = pdlGetPathToSoundFiles() + std::string("Glass.wav");
    std::cout << filePath << std::endl;
    soundBuffer.loadSoundFile(filePath.c_str());
    //new amplitude targets will arrive smoothly over 100ms
    amplitude.setTarget(100.0f);
    amplitude.setTarget(1.0f);

    //add a few sliders for control
    pdlAddSlider(app, 0, "Portamento (ms)", 0.0f, 2000.0f, 250.0f);
    pdlAddSlider(app, 1, "Output Gain (dB)", -60.0f, 0.0f, -10.0f);
    
    pdlStartExampleApp(app);//start the app
    while(pdlRunExampleApp(app)){//while the app shouldn't quit
        pdlUpdateExampleApp(app);//update the app
    }
    //if the previous while loop has ended, it is time to close the app
    pdlDeleteExampleApp(app);
}