#include "pedal/pedal.hpp"
#include "example_app.hpp"

int main() {
    pdlHello();
    pdlExampleApp* app = pdlInitExampleApp();
    if (!app) {
        return 1;
    }
    while (pdlRunExampleApp(app)) {
        pdlUpdateExampleApp(app);
    }
    pdlDeleteExampleApp(app);
}
