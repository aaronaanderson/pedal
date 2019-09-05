#include "example_app.hpp"

#include "GL/gl3w.h"
#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "RtAudio.h"

#include <iostream>

struct pdlExampleApp {
    GLFWwindow* window;
};

pdlExampleApp* pdlInitExampleApp() {
    std::cout << "init" << std::endl;
    if (!glfwInit()) {
        std::cerr << "Fail: glfwInit\n";
        return nullptr;
    }

    glfwDefaultWindowHints();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, true);  // if OSX, this is a must
    glfwWindowHint(GLFW_AUTO_ICONIFY, false);  // so fullcreen does not iconify
    auto* window = glfwCreateWindow(640, 480, "Pedal", nullptr, nullptr);
    if (!window) {
    }
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    if (gl3wInit() != 0) {
        glfwDestroyWindow(window);
        glfwTerminate();
        return nullptr;
    }

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");

    auto* app = new pdlExampleApp;
    app->window = window;
    return app;
}

bool pdlRunExampleApp(pdlExampleApp* app) {
    glfwSwapBuffers(app->window);
    glfwPollEvents();
    return glfwWindowShouldClose(app->window)? false : true;
}

void pdlUpdateExampleApp(pdlExampleApp* app) {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    ImGui::Begin("Another Window");
    ImGui::Text("Hello from another window!");
    ImGui::End();
    ImGui::Render();

    int display_w, display_h;
    glfwGetFramebufferSize(app->window, &display_w, &display_h);
    glViewport(0, 0, display_w, display_h);
    glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void pdlDeleteExampleApp(pdlExampleApp* app) {
    std::cout << "delete" << std::endl;
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    glfwDestroyWindow(app->window);
    glfwTerminate();
    delete app;
}
