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

struct pdlExampleApp {
    GLFWwindow* window;
    RtAudio audio;
    std::string device_name;
    unsigned device_id;
    unsigned output_channels;
    unsigned sampling_rate;
    unsigned buffer_size;
    pdlExampleCallback callback;
};

static int audioCallback(void *outputBuffer, void *inputBuffer,
                         unsigned int nFrames, double streamTime,
                         RtAudioStreamStatus status, void *userData) {
    float* out = (float*)outputBuffer;
    for (unsigned i = 0; i < nFrames; i += 1) {

    }
    auto* app = (pdlExampleApp*)userData;
    if (app->callback) {
        app->callback(out, nFrames, app->sampling_rate, app->output_channels, streamTime);
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
    }
    glfwMakeContextCurrent(app->window);
    glfwSwapInterval(1);

    if (gl3wInit() != 0) {
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
    ImGui::SetNextWindowSize(ImVec2{display_w/2.0f, float(display_h)});
    ImGui::Begin("Left Window", nullptr, flags);
    ImGui::TextUnformatted(app->device_name.c_str());
    ImGui::Value("channels", app->output_channels);
    ImGui::Value("sampling rate", app->sampling_rate);
    ImGui::Value("buffer size", app->buffer_size);
    ImGui::End();

    ImGui::SetNextWindowPos(ImVec2{display_w/2.0f,0.0f});
    ImGui::SetNextWindowSize(ImVec2{display_w/2.0f, float(display_h)});
    ImGui::Begin("Right Window", nullptr, flags);
    ImGui::TextUnformatted(app->device_name.c_str());
    ImGui::Value("channels", app->output_channels);
    ImGui::Value("sampling rate", app->sampling_rate);
    ImGui::Value("buffer size", app->buffer_size);
    ImGui::End();

    ImGui::Render();

    glViewport(0, 0, display_w, display_h);
    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
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
