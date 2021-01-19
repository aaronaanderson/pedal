/*
// This blank example is included to show how to use the 
// limited example app framework
*/

#include "example_app.hpp"

//using namespace pedal;

void midiCallback(double deltaTime, std::vector<unsigned char>* message, app::PedalExampleApp* app){
    //This will be called whenver a midi message is received.
    //The message will be delivered in binary. 
    //Use pedal's MidiEvent class to convert this binary data to something usable
    //tell the app what to do with MIDI input here
}

void keyboardCallback(int key, bool keyDown){
    //This function will be called whenever the qwerty keyboard is interacted with.
    //Use key to determine the ascii code of the key pressed
    //Use keyDown to determine if the key was pressed or released
    //tell the application what to do with keyboard input here
}

void audioCallback(float* output, float* input, int bufferSize, int inputChannels, int outputChannels, app::PedalExampleApp* app){
    //This is the audio loop. This function will be called as needed automatically.

    //Audio arrives in the input array. 
    //Samples will be interleaved when they arrive
    //Accessing data outside of the array will likely cause a crash
    //Use the bufferSize and numInputChannels to make sure you are not out of bounds.

    //whatever is left in the output array will be passed to the sound card.
    //Samples should be interleaved.
    //accessing data outside of this array will likely cause a crash
    //Use the bufferSize and numOutputChannels to makre sure you are not out of bounds

    //Here is an example function that fills the output buffer with silence
    for(int sampleIndex = 0; sampleIndex < bufferSize; sampleIndex++){
        for(int channelIndex = 0; channelIndex < outputChannels; channelIndex++){
            output[sampleIndex * outputChannels + channelIndex] = 0.0f;
        }
    }
}

int main(){
    //Create the application (an audio callback is required here)
    app::PedalExampleApp* app = app::pdlInitializeExampleApp(audioCallback);
    //If using a qwerty keyboard callback, add it
    app::setKeyboardCallback(keyboardCallback);
    //If using a MIDI input callback, add it
    app::setMidiCallback(app, midiCallback);
    //Select a MIDI port if needed
    app::openMidiPort(app, 1);
    //This is the perpetual loop; it will keep going until the window is closed
    app::startApp(app);
    while(app::shouldContinue(app)){//while the window is still open
        app::update(app);//continue running the app
    }
    //If this point is reached, the application is closing
    app::freeMemory(app);
}