#include "Core/Application.h"
#include <iostream>

int main() {
    std::cout << "MAIN START\n";

    Application app;

    std::cout << "BEFORE RUN\n";
    app.Run();
    std::cout << "AFTER RUN\n";
    return 0;
}
