#include "hzpch.h"
#include "Application.h"
#include "Log.h"
#include "Hazel/Input.h"
#include "glm/glm.hpp"  
#include "Hazel/Renderer/Shader.h"
#include "Hazel/Renderer/Renderer.h"

#include <GLFW/glfw3.h>

namespace Hazel {

// 这里绑定要this的原因是成员函数需要具体实例才能运行
#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)

    Application* Application::s_Instance = nullptr;

    Application::Application()         
    {
        HAZEL_CORE_ASSERT(!s_Instance, "Application already exists!");
        s_Instance = this;
        m_Window = std::unique_ptr<Window>(Window::Create()); // 这里是创建一个Window
        m_Window->SetEventCallback(BIND_EVENT_FN(OnEvent)); // 这里是绑定OnEvent函数，当窗口收到事件时，就会调用OnEvent函数。

        m_ImGuiLayer = new ImGuiLayer();
        PushOverlay(m_ImGuiLayer);

    }

    Application::~Application() {
    }

    void Application::Run() {
        while (m_Running) {             
            float time = static_cast<float>(glfwGetTime()); //Platform::GetTime();
            Timestep timestep = time - m_LastFrameTime; // 这里可以相当于用右侧构造了一个Timestep对象
            m_LastFrameTime = time;

            // 这里从begin开始渲染，到end结束渲染。
            for (Layer* layer : m_LayerStack) {
                layer->OnUpdate(timestep);
            }
           
            m_ImGuiLayer->Begin();
			for (Layer* layer : m_LayerStack) {
				layer->OnImGuiRender();
			}
            m_ImGuiLayer->End();

            m_Window->OnUpdate();
        }
    }
    void Application::OnEvent(Event& e)
    {
        EventDispatcher dispatcher(e);
        dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(OnWindowClose));
        // HAZEL_CORE_TRACE("{0}", e);
        // 这里从end开始处理事件，到这个处理结束为止。
        for (auto it = m_LayerStack.end(); it != m_LayerStack.begin();) {
            (*--it)->OnEvent(e); // 这里是Layer的OnEvent接口，不是Application的OnEvent接口。
            if (e.IsHandled()) {
                break;
            }
        }

        auto [x, y] = Input::GetMousePosition();
        // HAZEL_CORE_TRACE("{0}, {1}", x, y);
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
