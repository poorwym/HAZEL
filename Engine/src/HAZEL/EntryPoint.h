#pragma once

/**
 * @note: 这里是程序进入的入口，目前基本上就是Init然后进行Run，然后进行Destroy。
 */

#if defined(HAZEL_PLATFORM_WINDOWS) || defined(HAZEL_PLATFORM_LINUX) || defined(HAZEL_PLATFORM_MACOS)

extern Hazel::Application* Hazel::CreateApplication();
int main(int argc, char** argv){
    Hazel::Log::Init();
    HAZEL_CORE_WARN("Initialized Log!");
    HAZEL_INFO("Initialized Log!");
    auto app = Hazel::CreateApplication();
    app->Run();
    delete app;
    return 0;
}

#endif

