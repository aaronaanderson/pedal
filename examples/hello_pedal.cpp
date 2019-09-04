#include "pedal/pedal.hpp"
#include "example_app.hpp"

int main() {
    pdlHello();
    pdlExampleApp* app = pdlInitExampleApp();
    while (pdlRunExampleApp(app)) {
    }
    pdlDeleteExampleApp(app);
}
