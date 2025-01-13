#include <Hazel.h>

class SandboxApp : public Hazel::Application
{
public:
    SandboxApp()
    {
    }

    ~SandboxApp()
    {
    }
};

Hazel::Application* Hazel::CreateApplication()
{
    return new SandboxApp();
}
