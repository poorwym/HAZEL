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
        //HAZEL_INFO("ExampleLayer::Update");// 更新逻辑
    }

    void OnEvent(Hazel::Event& event) override
    {
        // HAZEL_TRACE("ExampleLayer::Event: {0}", event); // 事件处理逻辑
        if (event.GetEventType() == Hazel::EventType::KeyPressed) {
            auto& e = (Hazel::KeyPressedEvent&)event;
            if (e.GetKeyCode() == HAZEL_KEY_TAB) {
                HAZEL_INFO("Tab key is pressed!");
            }
            HAZEL_TRACE("{0}", (char)e.GetKeyCode());
        }
    }
};

class SandboxApp : public Hazel::Application
{
public:
    SandboxApp()
    {
        PushLayer(new ExampleLayer());
        // PushOverlay(new Hazel::ImGuiLayer());
    }

    ~SandboxApp()
    {
    }
};

Hazel::Application* Hazel::CreateApplication()
{
    return new SandboxApp();
}
