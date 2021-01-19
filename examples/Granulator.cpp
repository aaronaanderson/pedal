/*
// This blank example is included to show how to use the 
// limited example app framework
*/

#include "example_app.hpp"


#include <iostream>
#include <vector>


#include "pedal/Window.hpp"
#include "pedal/Buffer.hpp"
#include "pedal/BufferPlayer.hpp"
#include "pedal/ImpulseGenerator.hpp"
#include "pedal/Phasor.hpp"
#include "pedal/utilities.hpp"

using namespace pedal;

Buffer buffer;//store and audio file

//We will make a simple 'grain' definition
struct Grain{
    Window window;
    BufferPlayer bufferPlayer;
};
const int maximumNumberOfGrains = 20;

//create an array of grains
std::vector<Grain> grainArray(maximumNumberOfGrains, Grain());

//make an audio-rate timer to determine if a new grain should be emitted 
ImpulseGenerator impulseGenerator;

//Smooth value to scale the output
SmoothValue<float> outputGain;

void audioCallback(float* output, float* input, int bufferSize, int inputChannels, int outputChannels, PedalExampleApp* app){
   
    impulseGenerator.setFrequency(pdlGetSlider(app, 0));
    outputGain.setTarget(dBToAmplitude(pdlGetSlider(app, 4)));
    for(int sampleIndex = 0; sampleIndex < bufferSize; sampleIndex++){
        //progress the grain index location (in case a new grain is to be made)
        //phasor.generateSample();
        //Check the impulse generator to see if a new grain should be made
        if(impulseGenerator.generateSample() == 1.0f){
            //we'll just loop through them all and hope we find one
            for(int i = 0; i < maximumNumberOfGrains; i++){
                if(grainArray[i].window.getActive() == false){
                    //if the grain is inactive, so index 'i' is safe to use
                    grainArray[i].window.setDuration(pdlGetSlider(app, 1));
                    grainArray[i].window.setTrigger(true);
                    //grainArray[i].bufferPlayer.setPosition(0.0f);
                    grainArray[i].bufferPlayer.setSpeed(pdlGetSlider(app, 3));
                    grainArray[i].bufferPlayer.setPosition(pdlGetSlider(app, 2) * //normalized index
                                                           buffer.getDuration());//scale by duraction of buffer
                    grainArray[i].bufferPlayer.play();
                    break;
                }
            }//if we don't find one, we give up! Maybe next time, little grain
        }
        float currentSample = 0.0f; //single sample to work with
        for(int i = 0; i < maximumNumberOfGrains; i++){
            if(grainArray[i].window.getActive()){
                currentSample += grainArray[i].bufferPlayer.update() * grainArray[i].window.generateSample();
            
            }
        }
        currentSample *= outputGain.process();
        for(int channelIndex = 0; channelIndex < outputChannels; channelIndex++){
            output[sampleIndex * outputChannels + channelIndex] = currentSample;
        }
    }
}

int main(){
    
    std::string filePath = pdlGetPathToSoundFiles() + std::string("Glass.wav");
    buffer.loadSoundFile(filePath.c_str());
    //now we can set the frequency of the phasor based on the duration of the file
    for(int i = 0; i < maximumNumberOfGrains; i++){
        grainArray[i].bufferPlayer.setReference(&buffer);
        grainArray[i].bufferPlayer.setPlayMode(BufferPlayer::PlayMode::LOOP);
        grainArray[i].bufferPlayer.setSpeed(1.0f);
        grainArray[i].window.setDuration(500.0f);
        grainArray[i].window.setMode(Window::Mode::HANNING);
    }
    //Create the application (an audio callback is required here)
    PedalExampleApp* app = pdlInitializeExampleApp(audioCallback);
    //we need to set the references for all of the bufferPlayers
    pdlAddSlider(app, 0, "Emission Rate", 0.5f, 40.0f, 20.0f);
    pdlAddSlider(app, 1, "Grain Duration", 5.0f, 80.0f, 60.0f);
    pdlAddSlider(app, 2, "Sample Index", 0.0f, 1.0f, 0.2f);
    pdlAddSlider(app, 3, "Speed", 0.5f, 2.0f, 1.0f);
    pdlAddSlider(app, 4, "Output Gain", -60.0f, 0.0f, -6.0f);

    pdlStartExampleApp(app);
    while(pdlRunExampleApp(app)){//while the window is still open
        pdlUpdateExampleApp(app);//continue running the app
    }
    //If this point is reached, the application is closing
    pdlDeleteExampleApp(app);
}