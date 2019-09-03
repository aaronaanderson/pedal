#include "example_app.hpp"
#include "GL/gl3w.h"
#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"
#include "imgui.h"

#include <iostream>

struct pdlExampleApp {
};

pdlExampleApp* pdlInitExampleApp() {
    std::cout << "init" << std::endl;
    return new pdlExampleApp;
}

void pdlDeleteExampleApp(pdlExampleApp* app) {
    std::cout << "delete" << std::endl;
    delete app;
}
