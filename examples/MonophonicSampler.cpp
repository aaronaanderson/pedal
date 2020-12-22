/*
//
// A simple sound file player with playback speed
// controlled by note value
//
// The note may be determined by MIDI controller, or 
// by qwerty (ascii) keyboard
//
// Aaron Anderson, Oct 2020
*/

#include "example_app.hpp"
#include <iostream>

#include "pedal/MIDIEvent.hpp"
#include "pedal/utilities.hpp"
#include "pedal/Buffer.hpp"
#include "pedal/BufferPlayer.hpp"

Buffer soundBuffer;//place to store the sound file
BufferPlayer bufferPlayer(&soundBuffer);//simple buffer player
SmoothValue<float> noteAmplitudeScalar;
SmoothValue<float> outputGain;

void midiCallback(double deltaTime, std::vector<unsigned char>* message, PedalExampleApp* app){
    MIDIEvent event(message);
    switch(event.getEventType()){
        case MIDIEvent::EventTypes::NOTE_ON:
            //set amplitude scalar based on note velocity
            noteAmplitudeScalar.setTarget(dBToAmplitude((127 - event.getNoteVelocity()) * -0.1f));
            //set the buffer speed based on note number
            bufferPlayer.setSpeed(midiNoteToPlaySpeed(event.getNoteNumber()));
            bufferPlayer.stop();//reset the buffer player
            bufferPlayer.play();//restart the buffer player
        break;
        case MIDIEvent::EventTypes::NOTE_OFF:
        //nothing to do here
        break;
  }
}

void keyboardCallback(int key, bool keyDown){
    int midiNoteNumber = pdlAsciiToMidi(key);//returns -1 if key invalid
    if(midiNoteNumber > -1){//if key mapped to midi note
        if(keyDown){
            bufferPlayer.setSpeed(midiNoteToPlaySpeed(midiNoteNumber));//set speed based on note value
            bufferPlayer.stop();//reset the bufferPlayer
            bufferPlayer.play();//restart the bufferPlayer
        }else{//key up
        //nothing to do here
        }
    }
}

void audioCallback(float* output, float* input, int bufferSize, int outputChannels, int inputChannels, PedalExampleApp* app){
    //collect these values once per buffer (very lazy form of control rate)
    float outputGainScalar = dBToAmplitude(pdlGetSlider(app, 0));// collect the 'volume' scalar (noteAmplitudeScalar scalar)
    outputGain.setTarget(outputGainScalar);
    for(int sampleIndex = 0; sampleIndex < bufferSize; sampleIndex++){//for every sample
        float currentSample = bufferPlayer.update();//collect the next sample from the buffer
        currentSample *= noteAmplitudeScalar.process();//scale based on note velocity
        currentSample *= outputGain.process();//scale based on slider value
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
    soundBuffer.loadSoundFile(filePath.c_str());

    bufferPlayer.setPlayMode(PlayMode::ONE_SHOT);
    //new noteAmplitudeScalar targets will arrive smoothly over 100ms
    noteAmplitudeScalar.setTarget(100.0f);
    noteAmplitudeScalar.setTarget(1.0f);

    //add a few sliders for control
    pdlAddSlider(app, 0, "Output Gain (dB)", -60.0f, 0.0f, -10.0f);
    
    pdlStartExampleApp(app);//start the app
    while(pdlRunExampleApp(app)){//while the app shouldn't quit
        pdlUpdateExampleApp(app);//update the app
    }
    //if the previous while loop has ended, it is time to close the app
    pdlDeleteExampleApp(app);
}