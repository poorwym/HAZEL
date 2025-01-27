#pragma once

#include "Hazel/Window.h"
#include "Hazel/Renderer/GraphicsContext.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace Hazel {
    class HAZEL_API WindowsWindow : public Window {
    // 基本上就是对抽象窗口的一些实现
    public:
        WindowsWindow(const WindowProps& props);
        virtual ~WindowsWindow();

        void OnUpdate() override;

        inline unsigned int GetHeight() const override { return m_Data.Height; };
        inline unsigned int GetWidth() const override { return m_Data.Width; };

        // Window attributes
        inline void SetEventCallback(const EventCallbackFn& callback) override { m_Data.EventCallback = callback; };
        void SetVSync(bool enabled) override;
        bool IsVSync() const override;

        // 获取原生窗口
        inline virtual void* GetNativeWindow() const { return m_Window; };
    private:
        virtual void Init(const WindowProps& props);
        virtual void Shutdown();
    private:
        GLFWwindow* m_Window;
        GraphicsContext* m_Context;

        struct WindowData {
            std::string Title;
            unsigned int Width;
            unsigned int Height;
            bool VSync;

            EventCallbackFn EventCallback;
        };

        WindowData m_Data;
    };
}