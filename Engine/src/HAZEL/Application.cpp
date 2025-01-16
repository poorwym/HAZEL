#include "hzpch.h"
#include "Application.h"
#include "Log.h"

namespace Hazel {
// 这里绑定要this的原因是成员函数需要具体实例才能运行
#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)
    Application::Application() {
        m_Window = std::unique_ptr<Window>(Window::Create());
        m_Window->SetEventCallback(BIND_EVENT_FN(OnEvent));
    }

    Application::~Application() {
    }

    void Application::Run() {
        while (m_Running) { 
            // 这里从begin开始渲染，到end结束渲染。
            for (Layer* layer : m_LayerStack) {
                layer->OnUpdate();
            }
            m_Window->OnUpdate();
        }
    }
    void Application::OnEvent(Event& e)
    {
        EventDispatcher dispatcher(e);
        dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(OnWindowClose));
        HAZEL_CORE_TRACE("{0}", e);

        // 这里从end开始处理事件，到这个处理结束为止。
        for (auto it = m_LayerStack.end(); it != m_LayerStack.begin();) {
            (*--it)->OnEvent(e); // 这里是Layer的OnEvent接口，不是Application的OnEvent接口。
            if (e.IsHandled()) {
                break;
            }
        }
    }

    void Application::PushLayer(Layer* layer)
    {
        m_LayerStack.PushLayer(layer);
    }

    void Application::PushOverlay(Layer* overlay)
    {
        m_LayerStack.PushOverlay(overlay);
    }

    bool Hazel::Application::OnWindowClose(WindowCloseEvent& e)
    {
        m_Running = false;
        return true;
    }
}
