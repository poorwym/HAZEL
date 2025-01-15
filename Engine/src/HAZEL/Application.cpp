#include "hzpch.h"
#include "Application.h"
#include "Event/ApplicationEvent.h"
#include "Log.h"

namespace Hazel {
    Application::Application() {
    }

    Application::~Application() {
    }

    void Application::Run() {
        WindowResizeEvent e(1920, 1080);
        HAZEL_TRACE(e.ToString());
        std::cout << "Hello, World!" << std::endl;
        //std::cout << e << std::endl;
    }
}
