#include "example_app.hpp"

#include "GL/gl3w.h"
#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "RtAudio.h"

#include <iostream>
#include <string>
#include <atomic>

#define NUM_SLIDERS_MAX 16
#define NUM_TOGGLES_MAX 16
#define NUM_TRIGGERS_MAX 16

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

struct pdlExampleApp {
    GLFWwindow* window;
    RtAudio audio;
    std::string device_name;
    unsigned device_id;
    unsigned output_channels;
    unsigned sampling_rate;
    unsigned buffer_size;
    pdlExampleCallback callback;
    slider sliders[NUM_SLIDERS_MAX];
    toggle toggles[NUM_TOGGLES_MAX];
    trigger triggers[NUM_TRIGGERS_MAX];
};

static int audioCallback(void *outputBuffer, void *inputBuffer,
                         unsigned int nFrames, double streamTime,
                         RtAudioStreamStatus status, void *userData) {
    float* out = (float*)outputBuffer;
    for (unsigned i = 0; i < nFrames; i += 1) {

    }
    auto* app = (pdlExampleApp*)userData;
    if (app && app->callback) {
        app->callback(out, nFrames, app->sampling_rate, app->output_channels,
                      streamTime, app);
    }
    return 0;
}

pdlExampleApp* pdlInitExampleApp(pdlExampleCallback callback) {
    auto* app = new pdlExampleApp;
    if (!app) {
        std::cerr << "Fail: app creation\n";
        return nullptr;
    }
    app->callback = callback;

    std::cout << "init" << std::endl;
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
    app->sampling_rate = device_info.preferredSampleRate;
    app->buffer_size = 512;

    RtAudio::StreamParameters p;
    p.deviceId = app->device_id;
    p.nChannels = app->output_channels;
    p.firstChannel = 0;
    try {
        app->audio.openStream(&p, nullptr, RTAUDIO_FLOAT32,
                              app->sampling_rate, &app->buffer_size,
                              audioCallback, app,
                              nullptr, nullptr); // option & error callback
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

void pdlStartExampleApp(pdlExampleApp* app) {
    app->audio.startStream();
}

bool pdlRunExampleApp(pdlExampleApp* app) {
    glfwSwapBuffers(app->window);
    glfwPollEvents();
    return glfwWindowShouldClose(app->window)? false : true;
}

void pdlUpdateExampleApp(pdlExampleApp* app) {
    int display_w, display_h;
    glfwGetFramebufferSize(app->window, &display_w, &display_h);
    int window_w, window_h;
    glfwGetWindowSize(app->window, &window_w, &window_h);

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
    ImGui::TextUnformatted(app->device_name.c_str());
    ImGui::Value("channels", app->output_channels);
    ImGui::Value("sampling rate", app->sampling_rate);
    ImGui::Value("buffer size", app->buffer_size);
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
    ImGui::End();

    ImGui::SetNextWindowPos(ImVec2{window_w/2.0f,0.0f});
    ImGui::SetNextWindowSize(ImVec2{window_w/2.0f, float(window_h)});
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
}

void pdlDeleteExampleApp(pdlExampleApp* app) {
    std::cout << "delete" << std::endl;
    try {
        app->audio.stopStream();
    }
    catch (RtAudioError& e) {
        e.printMessage();
    }
    if (app->audio.isStreamOpen()) {
        app->audio.closeStream();
    }
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    glfwDestroyWindow(app->window);
    glfwTerminate();
    delete app;
}

void pdlAddSlider(pdlExampleApp* app, int sliderIndex, const char* name,
                  float low, float high, float initialValue) {
    slider* s = app->sliders + sliderIndex;
    s->name = name;
    s->low = low;
    s->high = high;
    s->val = initialValue;
    s->atomic_val.store(initialValue);
}

float pdlGetSlider(pdlExampleApp* app, int idx) {
    return app->sliders[idx].atomic_val.load();
}

void pdlAddToggle(pdlExampleApp* app, int toggleIndex, const char* name,
                  bool initialValue) {
    toggle* t = app->toggles + toggleIndex;
    t->name = name;
    t->val = initialValue;
    t->atomic_val.store(initialValue);
}

bool pdlGetToggle(pdlExampleApp* app, int idx) {
    return app->toggles[idx].atomic_val.load();
}

void pdlAddTrigger(pdlExampleApp* app, int triggerIndex, const char* name) {
    trigger* t = app->triggers + triggerIndex;
    t->name = name;
    t->val = false;
    t->atomic_val.store(false);
}

bool pdlGetTrigger(pdlExampleApp* app, int idx) {
    return app->triggers[idx].atomic_val.exchange(false);
}
