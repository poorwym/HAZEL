#include <Hazel.h>

class ExampleLayer : public Hazel::Layer
{
public:
    ExampleLayer()
        : Layer("Example")
    {
    }

    void OnUpdate() override
    {
        HAZEL_INFO("ExampleLayer::Update");// 更新逻辑
    }

    void OnEvent(Hazel::Event& event) override
    {
        HAZEL_TRACE("ExampleLayer::Event: {0}", event); // 事件处理逻辑
    }
};

class SandboxApp : public Hazel::Application
{
public:
    SandboxApp()
    {
        PushLayer(new ExampleLayer());
    }

    ~SandboxApp()
    {
    }
};

Hazel::Application* Hazel::CreateApplication()
{
    return new SandboxApp();
}
