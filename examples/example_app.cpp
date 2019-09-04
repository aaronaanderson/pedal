#include "example_app.hpp"
#include "GL/gl3w.h"
#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"
#include "imgui.h"
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
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    auto* app = new pdlExampleApp;
    app->window = window;
    return app;
}

bool pdlRunExampleApp(pdlExampleApp* app) {
    glfwSwapBuffers(app->window);
    glfwPollEvents();
    return glfwWindowShouldClose(app->window)? false : true;
}

void pdlDeleteExampleApp(pdlExampleApp* app) {
    std::cout << "delete" << std::endl;
    glfwDestroyWindow(app->window);
    glfwTerminate();
    delete app;
}
