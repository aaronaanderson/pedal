/*
// A simple monophonic synth. Monophonic synths, or synths that play
// one note at a time, offer a glimpse at a synthesizer with significantly
// reduced complexity. 

// A midi keyboard can be used to play this synth. For convenience, the ascii
// keyboard will supplement as input.
*/

#include "example_app.hpp"
#include <iostream>


void midiCallback(double deltaTime, std::vector<unsigned char>* message, PedalExampleApp* app){
    std::cout << "midi working" << std::endl;
}

void keyboardCallback(int key, bool keyDown){
    if(keyDown){
        std::cout << key << std::endl;
    }
}

void audioCallback(float* output, float* input, unsigned bufferSize, unsigned sampleRate, 
                   unsigned outputChannels, unsigned inputChannels, double time, PedalExampleApp* app){

}

int main(){
    PedalExampleApp* app = pdlInitializeExampleApp(audioCallback, midiCallback);
    pdlSetKeyboardCallback(keyboardCallback);
    pdlOpenMidiPort(app, 1);
    while(pdlRunExampleApp(app)){
        pdlUpdateExampleApp(app);
    }
    pdlDeleteExampleApp(app);
}