#include "Application.h"
#include <iostream>

namespace Hazel {
    Application::Application() {
    }

    Application::~Application() {
    }

    void Application::Run() {
        std::cout << "Hello, World!" << std::endl;
    }
}
