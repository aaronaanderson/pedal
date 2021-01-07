/*
// This blank example is included to show how to use the 
// limited example app framework
*/

#include "example_app.hpp"
#include "pedal/LowFrequencyOscillator.hpp"

using namespace pedal;
LowFrequencyOscillator lfo;
TSine test;
void midiCallback(double deltaTime, std::vector<unsigned char>* message, PedalExampleApp* app){

}

void keyboardCallback(int key, bool keyDown){

}

void audioCallback(float* output, float* input, int bufferSize, int inputChannels, int outputChannels, PedalExampleApp* app){

    for(int sampleIndex = 0; sampleIndex < bufferSize; sampleIndex++){
        float currentSample = lfo.generateSample() * 0.1f;
        for(int channelIndex = 0; channelIndex < outputChannels; channelIndex++){
            output[sampleIndex * outputChannels + channelIndex] = lfo.getSample();
        }
    }
}

int main(){
    //Create the application (an audio callback is required here)
    PedalExampleApp* app = pdlInitializeExampleApp(audioCallback);

    pdlStartExampleApp(app);
    //This is the perpetual loop; it will keep going until the window is closed
    while(pdlRunExampleApp(app)){//while the window is still open
        pdlUpdateExampleApp(app);//continue running the app
    }
    //If this point is reached, the application is closing
    pdlDeleteExampleApp(app);
}