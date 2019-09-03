#include "example_app.hpp"

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
