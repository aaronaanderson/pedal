#include "example_app.hpp"

#include "GL/gl3w.h"
#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "RtAudio.h"
#include "RtMidi.h"

#include <iostream>
#include <string>
#include <atomic>

#define NUM_SLIDERS_MAX 16
#define NUM_TOGGLES_MAX 16
#define NUM_TRIGGERS_MAX 16
#define NUM_DROPDOWNS_MAX 16

struct slider {
    std::string name;
    std::atomic<float> atomic_val;
    float low, high;
    float val;
};

struct toggle {
    std::string name;
    std::atomic<bool> atomic_val;
    bool val;
};

struct trigger {
    std::string name;
    std::atomic<bool> atomic_val;
    bool val;
};

struct dropDown{
    std::string name;
    std::atomic<int> atomic_val;
    char** content;
    int length;
    int val;
};

struct PedalExampleApp {
    GLFWwindow* window;
    RtAudio audio;
    std::string device_name;
    unsigned device_id;
    unsigned input_channels;
    unsigned output_channels;
    unsigned sample_rate;
    unsigned buffer_size;
    pdlExampleAudioCallback callback;
    RtMidiIn* rtMidiIn = nullptr;
    std::string midiDeviceName;
    unsigned int numPorts;    
    int userDefinedPort = -1;//initiate with invalid port
    slider sliders[NUM_SLIDERS_MAX];
    toggle toggles[NUM_TOGGLES_MAX];
    trigger triggers[NUM_TRIGGERS_MAX];
    dropDown dropDowns[NUM_DROPDOWNS_MAX];
    std::atomic<float> cursorx;
    std::atomic<float> cursory;
};

//callback function for ascii keyboard
void (*customKeyboardCallback)(int keyPressed, bool keyDown) = nullptr;

static int audioCallback(void *outputBuffer, void *inputBuffer,
                         unsigned int nFrames, double streamTime,
                         RtAudioStreamStatus status, void *userData) {
    float* out = (float*)outputBuffer;
    float* in = (float*)inputBuffer;
    auto* app = (PedalExampleApp*)userData;
    if (app && app->callback) {
        app->callback(out, in, nFrames, app->output_channels,
                      app->input_channels, app);
    }
    return 0;
}

static float clampf01(float x) {
    if (x < 0.0f) return 0.0f;
    else if (x > 1.0f) return 1.0f;
    else return x;
}

PedalExampleApp* appPtr;
PedalExampleApp* getAppPtr(){return appPtr;}
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if ((action == GLFW_PRESS) && (mods & GLFW_MOD_CONTROL) && (key == GLFW_KEY_Q)) {
        glfwSetWindowShouldClose(window, 1);
    }else if(action == GLFW_PRESS && (mods &GLFW_MOD_CONTROL) && (key >= 48) && (key <=57)){// if ctl + number
        // // ascii number on keyboard is n + 48
        // int deviceIndex = key - 48;
        // //check to make sure that it isn't already running
        // if(getAppPtr()->audio.isStreamOpen())
        //     getAppPtr()->audio.closeStream();
        // }
        // if(deviceIndex < getAppPtr()->audio.getDeviceCount()){
        //     getAppPtr()->audio.
        // }
        
    }
    bool keyDown;
    if(action == GLFW_PRESS || action == GLFW_RELEASE){
        keyDown = (action == GLFW_PRESS) ? true : false;
        if(*customKeyboardCallback != nullptr){
            customKeyboardCallback(key, keyDown);
        }
    }
}

PedalExampleApp* pdlInitializeExampleApp(pdlExampleAudioCallback callback, int sampleRate, int bufferSize) {
    auto* app = new PedalExampleApp;
    appPtr = app;
    if (!app) {
        std::cerr << "Fail: app creation\n";
        return nullptr;
    }
    app->callback = callback;

    if (!glfwInit()) {
        std::cerr << "Fail: glfwInit\n";
        delete app;
        return nullptr;
    }

    glfwDefaultWindowHints();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, true);  // if OSX, this is a must
    glfwWindowHint(GLFW_AUTO_ICONIFY, false);  // so fullcreen does not iconify
    app->window = glfwCreateWindow(640, 480, "Pedal", nullptr, nullptr);
    if (!app->window) {
        std::cerr << "Fail: glfwCreateWindow\n";
        glfwTerminate();
        delete app;
        return nullptr;
    }
    glfwSetKeyCallback(app->window, keyCallback);
    glfwMakeContextCurrent(app->window);
    glfwSwapInterval(1);

    if (gl3wInit() != 0) {
        std::cerr << "Fail: gl3wInit\n";
        glfwDestroyWindow(app->window);
        glfwTerminate();
        delete app;
        return nullptr;
    }

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGui_ImplGlfw_InitForOpenGL(app->window, true);
    ImGui_ImplOpenGL3_Init("#version 330");

    unsigned default_out = app->audio.getDefaultOutputDevice();

    auto device_info = app->audio.getDeviceInfo(default_out);
    app->device_id = default_out;
    app->device_name = device_info.name;
    
    app->output_channels = device_info.outputChannels;
    //app->sample_rate = device_info.preferredSampleRate;
    app->sample_rate = sampleRate;
    app->buffer_size = bufferSize;

    RtAudio::StreamParameters outputParameters;
    outputParameters.deviceId = app->device_id;
    outputParameters.nChannels = app->output_channels;
    
    outputParameters.firstChannel = 0;
    RtAudio::StreamParameters inputParameters;
    unsigned default_in = app->audio.getDefaultInputDevice();
    inputParameters.deviceId = default_in;
    device_info = app->audio.getDeviceInfo(default_in);
    app->input_channels = device_info.inputChannels;
    inputParameters.nChannels = app->audio.getDeviceInfo(default_in).inputChannels;
    inputParameters.firstChannel = 0;
    
    try {
        if(inputParameters.nChannels > 0){
            app->audio.openStream(&outputParameters, &inputParameters, RTAUDIO_FLOAT32,
            app->sample_rate, &app->buffer_size,
            audioCallback, app,
            nullptr, nullptr); // option & error callback
        }else{
            app->audio.openStream(&outputParameters, nullptr, RTAUDIO_FLOAT32,
            app->sample_rate, &app->buffer_size,
            audioCallback, app,
            nullptr, nullptr); // option & error callback
        }
    }
    catch (RtAudioError& e) {
        e.printMessage();
        glfwDestroyWindow(app->window);
        glfwTerminate();
        delete app;
        return nullptr;
    }    
    for (int i = 0; i < NUM_SLIDERS_MAX; i += 1) {
        slider* s = app->sliders + i;
        s->name = "";
        s->low = 0.0f;
        s->high = 1.0f;
        s->val = 0.0f;
        s->atomic_val.store(0.0f);
    }

    for (int i = 0; i < NUM_TOGGLES_MAX; i += 1) {
        toggle* t = app->toggles + i;
        t->name = "";
        t->val = false;
        t->atomic_val.store(false);
    }

    for (int i = 0; i < NUM_TRIGGERS_MAX; i += 1) {
        trigger* t = app->triggers + i;
        t->name = "";
        t->val = false;
        t->atomic_val.store(false);
    }

    return app;
}

void pdlStartExampleApp(PedalExampleApp* app) {
    try{
        app->audio.startStream();
    }catch(RtAudioError& e) {
        e.printMessage();
    }
}

bool pdlRunExampleApp(PedalExampleApp* app) {
    glfwSwapBuffers(app->window);
    glfwPollEvents();
    return glfwWindowShouldClose(app->window)? false : true;
}

void pdlUpdateExampleApp(PedalExampleApp* app) {
    int display_w, display_h;
    glfwGetFramebufferSize(app->window, &display_w, &display_h);
    int window_w, window_h;
    glfwGetWindowSize(app->window, &window_w, &window_h);
    double cursorx, cursory;
    glfwGetCursorPos(app->window, &cursorx, &cursory);

    float cx = (float)(cursorx / window_w);
    float cy = (float)(cursory / window_h);
    cx = clampf01(cx);
    cy = clampf01(cy);
    app->cursorx.store(cx);
    app->cursory.store(cy);

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    ImGuiWindowFlags flags = 0;
    flags |= ImGuiWindowFlags_NoTitleBar;
    flags |= ImGuiWindowFlags_NoResize;
    flags |= ImGuiWindowFlags_NoMove;
    flags |= ImGuiWindowFlags_NoCollapse;
    flags |= ImGuiWindowFlags_AlwaysAutoResize;
    flags |= ImGuiWindowFlags_NoBackground;
    flags |= ImGuiWindowFlags_NoSavedSettings;

    ImGui::SetNextWindowPos(ImVec2{0.0f,0.0f});
    ImGui::SetNextWindowSize(ImVec2{window_w/2.0f, float(window_h)});
    ImGui::Begin("Left Window", nullptr, flags);
    ImGui::TextUnformatted("ctrl-q to quit");
    ImGui::TextUnformatted(app->device_name.c_str());
    ImGui::Value("channels", app->output_channels);
    ImGui::Value("sampling rate", app->sample_rate);
    ImGui::Value("buffer size", app->buffer_size);
    ImGui::Value("mx", cx);
    ImGui::Value("my", cy);
    for (int i = 0; i < NUM_TOGGLES_MAX; i += 1) {
        toggle* t = app->toggles + i;
        if (t->name.empty()) continue;
        ImGui::Checkbox(t->name.c_str(), &t->val);
    }
    for (int i = 0; i < NUM_TRIGGERS_MAX; i += 1) {
        trigger* t = app->triggers + i;
        if (t->name.empty()) continue;
        t->val = ImGui::Button(t->name.c_str());
    }
    for(int i = 0; i < NUM_DROPDOWNS_MAX; i += 1){
        dropDown* t = app->dropDowns + i;
        if (t->name.empty()) continue;
        ImGui::Combo(t->name.c_str(), &t->val, t->content, t->length, 4);
    }//Combo(const char* label, int* current_item, const char* const items[], int items_count, int popup_max_height_in_items = -1);
    ImGui::End();

    ImGui::SetNextWindowPos(ImVec2{160.0f,0.0f});
    ImGui::SetNextWindowSize(ImVec2{window_w - 160, float(window_h)});
    ImGui::Begin("Right Window", nullptr, flags);
    for (int i = 0; i < NUM_SLIDERS_MAX; i += 1) {
        slider* s = app->sliders + i;
        if (s->name.empty()) continue;
        ImGui::SliderFloat(s->name.c_str(), &s->val, s->low, s->high);
    }
    ImGui::End();

    ImGui::Render();

    glViewport(0, 0, display_w, display_h);
    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    for (int i = 0; i < NUM_SLIDERS_MAX; i += 1) {
        slider* s = app->sliders + i;
        s->atomic_val.store(s->val);
    }

    for (int i = 0; i < NUM_TOGGLES_MAX; i += 1) {
        toggle* t = app->toggles + i;
        t->atomic_val.store(t->val);
    }

    for (int i = 0; i < NUM_TRIGGERS_MAX; i += 1) {
        trigger* t = app->triggers + i;
        t->atomic_val.store(t->val);
    }
    for (int i = 0; i < NUM_DROPDOWNS_MAX; i += 1){
        dropDown* t = app->dropDowns + i;
        t->atomic_val.store(t->val);
    }
}

void pdlDeleteExampleApp(PedalExampleApp* app) {
    try {
        app->audio.stopStream();
    }
    catch (RtAudioError& e) {
        e.printMessage();
    }
    if (app->audio.isStreamOpen()) {
        app->audio.closeStream();
    }
    delete app->rtMidiIn;
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    glfwDestroyWindow(app->window);
    glfwTerminate();
    delete app;
}
void pdlSetMidiCallback(PedalExampleApp* app, pdlExampleMidiInputCallback newMidiCallback){
    if(!app->rtMidiIn){
        app->rtMidiIn = new RtMidiIn();
    }
    // Check available ports.
    app->numPorts = app->rtMidiIn->getPortCount();
    std::cout << "The following MIDI ports are available:\n";
    for(int i = 0; i < app->numPorts; i++){
        std::cout << "Port " << i << ": " << app->rtMidiIn->getPortName(i) << std::endl;
    }
    if ( app->numPorts == 0 ) {
        std::cout << "No ports available!\n";
    }else{
        int port;
        if(app->userDefinedPort > -1 && app->userDefinedPort < app->numPorts){
            port = app->userDefinedPort;
        }else{
            port = 0;
        }
        app->rtMidiIn->openPort(port);
        // Set our callback function.  This should be done immediately after
        // opening the port to avoid having incoming messages written to the
        // queue.
        app->rtMidiIn->setCallback((RtMidiIn::RtMidiCallback)newMidiCallback);
        app->midiDeviceName = app->rtMidiIn->getPortName(port);
        // Don't ignore sysex, timing, or active sensing messages.
        app->rtMidiIn->ignoreTypes( true, false, false );
    }    
}
void pdlSetKeyboardCallback(void (*keyboardCallback)(int key, bool keyDown)){
    customKeyboardCallback = *keyboardCallback;
}
void pdlOpenMidiPort(PedalExampleApp* app, int port){
    app->userDefinedPort = port;
    if(app->userDefinedPort < app->rtMidiIn->getPortCount()){
        app->rtMidiIn->closePort();
        app->rtMidiIn->openPort(port);
        app->midiDeviceName = app->rtMidiIn->getPortName(port);
    }else{
        std::cout << "Port: " << port << " is not available" << std::endl;
    }
}
unsigned pdlGetSampleRate(PedalExampleApp* app) {
    return app->sample_rate;
}

unsigned pdlGetBufferSize(PedalExampleApp* app){
    return app->buffer_size;
}

void pdlGetCursorPos(PedalExampleApp* app, float* mx, float* my) {
    *mx = app->cursorx.load();
    *my = app->cursory.load();
}

void pdlAddSlider(PedalExampleApp* app, int sliderIndex, const char* name,
                  float low, float high, float initialValue) {
    slider* s = app->sliders + sliderIndex;
    s->name = name;
    s->low = low;
    s->high = high;
    s->val = initialValue;
    s->atomic_val.store(initialValue);
}

float pdlGetSlider(PedalExampleApp* app, int idx) {
    return app->sliders[idx].atomic_val.load();
}

void pdlAddToggle(PedalExampleApp* app, int toggleIndex, const char* name,
                  bool initialValue) {
    toggle* t = app->toggles + toggleIndex;
    t->name = name;
    t->val = initialValue;
    t->atomic_val.store(initialValue);
}

bool pdlGetToggle(PedalExampleApp* app, int idx) {
    return app->toggles[idx].atomic_val.load();
}

void pdlAddTrigger(PedalExampleApp* app, int triggerIndex, const char* name) {
    trigger* t = app->triggers + triggerIndex;
    t->name = name;
    t->val = false;
    t->atomic_val.store(false);
}

bool pdlGetTrigger(PedalExampleApp* app, int idx) {
    return app->triggers[idx].atomic_val.exchange(false);
}
void pdlAddDropDown(PedalExampleApp* app, int idx, const char* name,char* content[],int length){
    dropDown* t = app->dropDowns + idx;
    t->name = name;
    t->content = content;
    t->val = 0;
    t->length = length;
    t->atomic_val.store(0);
}
int pdlGetDropDown(PedalExampleApp* app, int idx){
    return app->dropDowns[idx].atomic_val.load();
}

int pdlAsciiToMidi(int asciiCode){
    //start with invalid midi note
    int midiNoteNumber = -1;
    if(asciiCode > 64 && asciiCode < 90){
        switch(asciiCode){
            case 65://a 
                midiNoteNumber = 48;//(one octave below middle C)
            break;
            case 87://w
                midiNoteNumber = 49;
            break;
            case 83://s
                midiNoteNumber = 50;
            break;
            case 69://e
                midiNoteNumber = 51;
            break;
            case 68://d
                midiNoteNumber = 52;
            break;
            case 70://f
                midiNoteNumber = 53;
            break;
            case 84://t
                midiNoteNumber = 54;
            break;
            case 71://g
                midiNoteNumber = 55;
            break;
            case 89://y
                midiNoteNumber = 56;
            break;
            case 72://h
                midiNoteNumber = 57;
            break;
            case 85://u
                midiNoteNumber = 58;
            break;
            case 74://j
                midiNoteNumber = 59;
            break;
            case 75://k
                midiNoteNumber = 60;//(middle C)
            break;
            default: //any other key is invalid
                midiNoteNumber = -1;
            break;
        }
    }
    return midiNoteNumber;
}